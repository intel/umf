package com.intel.vmf.rstp_server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Collections;
import java.util.List;

import net.majorkernelpanic.streaming.SessionBuilder;
import net.majorkernelpanic.streaming.gl.SurfaceView;
import net.majorkernelpanic.streaming.rtsp.RtspServer;
import net.majorkernelpanic.streaming.video.VideoQuality;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences.Editor;
import android.location.Location;
import android.location.LocationListener;
import android.os.Bundle;
import android.os.IBinder;
import android.os.SystemClock;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.TextView;
import android.widget.Toast;

import com.intel.vmf.Format;
import com.intel.vmf.FormatXML;
import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataSchema;
import com.intel.vmf.MetadataSet;
import com.intel.vmf.MetadataStream;
import com.intel.vmf.Variant;

public class MainActivity extends Activity implements  LocationListener {

	public static final String LOGTAG = "MainActivity";

	protected SurfaceView mSurfaceView;
	protected TextView mTextURL;
	protected String mIPString = "unknown";

	private ServiceConnection mRtspServiceConnection = new ServiceConnection() {

		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			Log.d(LOGTAG, "onServiceConnected");
			mRtspServer = ((RtspServer.LocalBinder)service).getService();
			mRtspServer.addCallbackListener(mRtspCallbackListener);
			mRtspServer.start();
			final String rtstAddr = "rtsp://" + mIPString + ":" + mRtspServer.getPort();
			Log.i(LOGTAG, "Ready for connection via " + rtstAddr);
			runOnUiThread(new Runnable() {
			    public void run() {
			    	mTextURL.setText("Connect " + rtstAddr);
			    	//Toast.makeText(MainActivity.this, rtstAddr, Toast.LENGTH_LONG).show();
			    }
			});
		}

		@Override
		public void onServiceDisconnected(ComponentName name) {
			Log.d(LOGTAG, "onServiceDisconnected");
		}
	};

	protected RtspServer mRtspServer;
	protected ServerSocket mServerSocket;
	protected Socket mSocket;
	protected DataOutputStream mSockOut;
	protected DataInputStream mSockIn;
	protected boolean mIsVMFStreamming = false;
	protected Format mVMFFormat = new FormatXML(); 
	protected String mStdSchemaString;
	protected String mVSegString;

	private RtspServer.CallbackListener mRtspCallbackListener = new RtspServer.CallbackListener() {

		@Override
		public void onError(RtspServer server, Exception e, int error) {
			if (error == RtspServer.ERROR_BIND_FAILED) {
				Log.e(LOGTAG, "RtspServer.ERROR_BIND_FAILED");
				runOnUiThread(new Runnable() {
				    public void run() {
				    	mTextURL.setText("Select another RTSP port");
				    	Toast.makeText(MainActivity.this, "The selected RSTP port is already in use!", Toast.LENGTH_LONG).show();
				    }
				});
			}
		}

		@Override
		public void onMessage(RtspServer server, int message) {
			if (message==RtspServer.MESSAGE_STREAMING_STARTED) {
				Log.i(LOGTAG, "MESSAGE_STREAMING_STARTED");
				runOnUiThread(new Runnable() {
				    public void run() {
				    	mTextURL.setVisibility(View.INVISIBLE);
				    	Toast.makeText(MainActivity.this, "STREAMING STARTED", Toast.LENGTH_LONG).show();
				    }
				});

				new Thread("Init VMF Metadata Streaming") {

					public void run() {
			            Log.i(LOGTAG, "Starting streaming VMF metadata");

			    		int VmfPort = 4321;
			    				/*Integer.parseInt(
			    						PreferenceManager.getDefaultSharedPreferences(getApplicationContext())
			    					.getString("vmfPort", "4321") );*/

						try {
							mServerSocket = new ServerSocket(VmfPort);
						} catch (IOException e) {
							final String msg = "Error creating ServerSocket("+VmfPort+"): " + e.getMessage();
							Log.e(LOGTAG, msg);
							runOnUiThread(new Runnable() {
							    public void run() {
							    	Toast.makeText(MainActivity.this, msg, Toast.LENGTH_LONG).show();
							    }
							});
						}

						while(!mIsVMFStreamming && mServerSocket != null) {
			    		try {
			    			Log.d(LOGTAG, "Waiting for VMF Metadata connection...");
				            mSocket = mServerSocket.accept();
				            Log.i(LOGTAG, "Connection accepted!");
				            mSockOut = new DataOutputStream(mSocket.getOutputStream());
				            mSockIn  = new DataInputStream (mSocket.getInputStream());

				            // handshake
				            Log.i(LOGTAG, "Starting handshake (VMF/VMF, XML/OK)");
				            // VMF/VMF
				            sendString("VMF"); 
				            byte[] buff = new byte[3];
				            Log.d(LOGTAG, "Waiting for 'VMF'...");
				            mSockIn.read(buff);
				            String s = new String(buff);
				            Log.d(LOGTAG, "Got " + s);
				            if( !s.equalsIgnoreCase("VMF") )
				            	throw new IOException("Invalid response ('VMF' expected): " + s);
				            // XML/OK
				            sendString("XML");
				            buff = new byte[2];
				            Log.d(LOGTAG, "Waiting for 'OK'");
				            mSockIn.read(buff);
				            s = new String(buff);
				            Log.d(LOGTAG, "Got " + s);
				            if( !s.equalsIgnoreCase("OK") )
				            	throw new IOException("Invalid response ('OK' expected): " + s);

				            Log.i(LOGTAG, "Sending VMF VideoSegment");
				            if(mVSegString == null) {
				            	mVSegString = mVMFFormat.store(
				            			null,
				            			null, 
			            				new MetadataStream.VideoSegment[]{new MetadataStream.VideoSegment("Android", 15, 1)}, 
			            				null); 
				            }
				            sendString(mVSegString);

				            Log.i(LOGTAG, "Sending VMF schema");
				            if(mStdSchemaString == null) {
				            	mStdSchemaString = mVMFFormat.store(
				            			null,
				            			new MetadataSchema[]{MetadataSchema.getStdSchema()},
				            			null,
				            			null);
				            }
				            sendString(mStdSchemaString);

				            Log.i(LOGTAG, "Ready to stream VMF medatada");
				            mIsVMFStreamming = true;

				            // TODO: if useFakeLocations
				            // fake locations generation
			            	MetadataSchema schema = MetadataSchema.getStdSchema();
			            	MetadataStream ms = new MetadataStream();
			            	ms.addSchema(schema);
			            	double stepAndgle = Math.PI / 20.;
			            	double stepRadius = 0;
			            	double angle = Math.PI * 2, radius = 0.;
			            	Variant lat = new Variant(), lng = new Variant();
			            	while(true) {
			            		angle -= stepAndgle;
			            		angle = angle < 0 ? angle + Math.PI * 2 : angle;
			            		if(radius > 0.1) stepRadius = -stepRadius;
			            		else if(radius < 1e4) stepRadius = Math.random() * 5e-4 + 1e-4;
			            		radius += stepRadius;
			            		lat.setTo(37.388305 + radius * Math.cos(angle));
			            		lng.setTo(-121.96463 + radius * Math.sin(angle));

				            	Metadata m = new Metadata(schema.findMetadataDesc("location"));
				            	m.setFieldValue("latitude", lat);
				            	m.setFieldValue("longitude", lng);
				            	ms.add(m);
				            	MetadataSet set = ms.getAll();
				            	sendString(mVMFFormat.store(set, null, null, null));
				            	ms.remove(set);

				            	SystemClock.sleep(1500);
			            	}
						} catch (Exception e) {
							Log.e(LOGTAG, "Error initializing VMF metadata streaming: " + e.getMessage());
							mIsVMFStreamming = false;
							try {
								if(mSocket != null) {
									mSocket.close();
									mSocket = null;
								}
							} catch (IOException e1) {
								Log.e(LOGTAG, "Error closing VMF client socket: " + e1.getMessage());
							}
			        	}
						}
				}}.start();

			} else if (message==RtspServer.MESSAGE_STREAMING_STOPPED) {
				Log.i(LOGTAG, "MESSAGE_STREAMING_STOPPED");
				runOnUiThread(new Runnable() {
				    public void run() {
				    	mTextURL.setVisibility(View.VISIBLE);
				    	Toast.makeText(MainActivity.this, "STREAMING STOPPED", Toast.LENGTH_LONG).show();
				    }
				});

				Log.i(LOGTAG, "Stop VMF metadata streaming");
				mIsVMFStreamming = false;
				mSockOut = null;
				mSockIn  = null;
				try {
					if(mSocket != null) {
						mSocket.close();
						mSocket = null;
					}
					if(mServerSocket != null) {
						mServerSocket.close();
						mServerSocket = null;
					}
				} catch (IOException e) {
					Log.e(LOGTAG, "Error closing VMF sockets: " + e.getMessage());
				}
			}
		}

	};


	@Override
    protected void onCreate(Bundle savedInstanceState) {
		Log.d(LOGTAG, "onCreate");
        super.onCreate(savedInstanceState);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		setContentView(R.layout.activity_main);
		mTextURL = (TextView) findViewById(R.id.textURL);
		mSurfaceView = (SurfaceView) findViewById(R.id.surface_preview);

		// RTSP init

		// Sets the port of the RTSP server to 1234
		Editor editor = PreferenceManager.getDefaultSharedPreferences(this).edit();
		editor.putString(RtspServer.KEY_PORT, String.valueOf(1234));
		editor.commit();

		SessionBuilder.getInstance()
		        .setSurfaceView(mSurfaceView)
		        .setPreviewOrientation(0)
		        .setContext(getApplicationContext())
		        .setAudioEncoder(SessionBuilder.AUDIO_NONE)
		        .setVideoEncoder(SessionBuilder.VIDEO_H263)
		        .setVideoQuality(new VideoQuality(640,480,15,2000000));

		startService(new Intent(this, RtspServer.class));
    }

	@Override
	protected void onDestroy() {
		Log.d(LOGTAG, "onDestroy");
		stopService(new Intent(this, RtspServer.class));
		super.onDestroy();
	}

	@Override
	public void onPause() {
		Log.d(LOGTAG, "onPause");
		stopStreaming();
		super.onPause();
	}

	@Override
	public void onResume() {
		Log.d(LOGTAG, "onResume");
		super.onResume();
		startStreaming();
	}

	protected void startStreaming() {
		// refresh IP
		try {
			 List<NetworkInterface> interfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
			 for (NetworkInterface i : interfaces) {
	             List<InetAddress> addrs = Collections.list(i.getInetAddresses());
	             for (InetAddress a : addrs)
	            	 if (!a.isLoopbackAddress() && (a instanceof Inet4Address))
	            		 mIPString = a.getHostAddress();
	         }
	    } catch (Exception e) {}

    	mTextURL.setText("Connecting to RTSPServer... ");
		bindService(new Intent(this, RtspServer.class), mRtspServiceConnection, Context.BIND_AUTO_CREATE);

		//TODO: mLocationManager...
	}

	protected void stopStreaming() {
		if(mRtspServer != null)
			mRtspServer.stop();
		// TODO: sync required
		unbindService(mRtspServiceConnection);
		mRtspServer = null;

		//TODO: mLocationManager.removeUpdates(this);
	}

	protected synchronized void sendString(String msg) throws IOException  {
        if(mSockOut != null && msg!= null && !msg.isEmpty()) {
        	Log.d(LOGTAG, "Sending VMF string: " + msg);
        	byte[] buff = msg.getBytes("UTF8");
        	mSockOut.writeInt(buff.length);
        	mSockOut.write(buff);
        }
    }

	@Override
	public void onLocationChanged(Location arg0) {
		// TODO: NYI
	}

	@Override
	public void onProviderDisabled(String arg0) {
	}

	@Override
	public void onProviderEnabled(String arg0) {
	}

	@Override
	public void onStatusChanged(String arg0, int arg1, Bundle arg2) {
	}

}
