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
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.IBinder;
import android.os.SystemClock;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.TextView;
import android.widget.Toast;

import com.intel.vmf.Compressor;
import com.intel.vmf.EncryptorDefault;
import com.intel.vmf.Format;
import com.intel.vmf.FormatCompressed;
import com.intel.vmf.FormatEncrypted;
import com.intel.vmf.FormatJSON;
import com.intel.vmf.FormatXML;
import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataSchema;
import com.intel.vmf.MetadataSet;
import com.intel.vmf.MetadataStream;
import com.intel.vmf.Variant;

public class MainActivity extends Activity implements  LocationListener {

	public static final String LOGTAG = "MainActivity";

	protected SurfaceView mSurfaceView;
	protected TextView mTextURL, mTextFormat, mTextGpsEmu, mTextCompress, mTextEncrypt;
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
			    	mTextURL.setVisibility(View.VISIBLE);
			    	mTextURL.setText("Connect " + rtstAddr);
			    	//Toast.makeText(MainActivity.this, rtstAddr, Toast.LENGTH_LONG).show();
			    }
			});
		}

		@Override
		public void onServiceDisconnected(ComponentName name) {
			Log.d(LOGTAG, "onServiceDisconnected");
			mRtspServer = null;
		}
	};

	protected LocationManager mLocationManager;
	protected boolean mUseGpsEmulation = true;
	protected RtspServer mRtspServer;
	protected ServerSocket mServerSocket;
	protected Socket mSocket;
	protected DataOutputStream mSockOut;
	protected DataInputStream mSockIn;
	protected boolean mIsVMFStreamming = false;
	protected Format mVMFFormat; 
	protected Metadata m = new Metadata(MetadataSchema.getStdSchema().findMetadataDesc("location"));
	protected MetadataSet set = new MetadataSet();
	protected Variant lat = new Variant(), lng = new Variant();
	
	protected Thread mGpsEmulationTread = null;
	protected Runnable mGpsEmulation = new Runnable() {
		@Override
		public void run() {
            // fake locations generation
            Log.i(LOGTAG, "STARTING streaming fake locations...");
        	double stepAndgle = Math.PI / 20., stepRadius = 0;
        	double angle = Math.PI * 2, radius = 0.;
        	try{
            	while(mIsVMFStreamming) {
            		if(Thread.currentThread().isInterrupted())
            			throw new Exception("Thread is interrupted");
            		angle -= stepAndgle;
            		angle = angle < 0 ? angle + Math.PI * 2 : angle;
            		if(radius > 0.1) stepRadius = -stepRadius;
            		else if(radius < 1e-4) stepRadius = Math.random() * 4e-4 + 1e-4;
            		radius += stepRadius;
            		lat.setTo(37.388305 + radius * Math.cos(angle));
            		lng.setTo(-121.96463 + radius * Math.sin(angle));
	            	m.setFieldValue("latitude", lat);
	            	m.setFieldValue("longitude", lng);
	            	m.setTimestamp( System.currentTimeMillis() );
	            	sendString(mVMFFormat.store(set, null, null, null, null));
	            	SystemClock.sleep(1000);
            	}
			} catch (Exception e) {
	            Log.e(LOGTAG, "Exception: " + e.getMessage());
			} finally {
	            Log.i(LOGTAG, "STOP streaming fake locations");
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
		}};

		protected Thread mVmfListeningTread = null;
		protected Runnable mVmfListening = new Runnable() {
			@Override
			public void run() {
	            Log.i(LOGTAG, "Starting streaming VMF metadata");
	            SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
	    		int VmfPort = Integer.parseInt( prefs.getString("vmf_port", "4321") );

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

				while(mServerSocket != null) {
	    		try {
	    			Log.d(LOGTAG, "Waiting for VMF Metadata connection on port " + VmfPort);
		            mSocket = mServerSocket.accept();
		            Log.i(LOGTAG, "Connection accepted!");
		            mSockOut = new DataOutputStream(mSocket.getOutputStream());
		            mSockIn  = new DataInputStream (mSocket.getInputStream());

		            final boolean useXML = prefs.getString("format", "1").equals("2");
		            Log.d(LOGTAG, "useXML="+(useXML?"yes":"no"));
		            final boolean useGpsEmu = prefs.getBoolean("gps_emulation", true);
		            final boolean useEncryption = prefs.getBoolean("encryption", false);
		            Log.d(LOGTAG, "useEncryption="+(useEncryption?"yes":"no"));
		            final boolean useCompression = prefs.getBoolean("compression", false);
		            Log.d(LOGTAG, "useCompression="+(useCompression?"yes":"no"));

					runOnUiThread(new Runnable() {
					    public void run() {
					    	mTextFormat.setText("Format: " + (useXML ? "XML" : "JSON"));
					    	mTextGpsEmu.setText("GPS emulation: " + (useGpsEmu ? "Yes" : "No"));
					    	mTextCompress.setText("Compression: " + (useCompression ? "Yes" : "No"));
					    	mTextEncrypt.setText("Encryption: " + (useEncryption ? "Yes" : "No"));
					    }
					});


		            mVMFFormat = useXML ? new FormatXML() : new FormatJSON();
		            mVMFFormat = useCompression ? new FormatCompressed(mVMFFormat, Compressor.BUILTIN_ZLIB) : mVMFFormat;
		            mVMFFormat = useEncryption ? new FormatEncrypted(mVMFFormat, new EncryptorDefault("VMF Demo passphrase!")) : mVMFFormat;

		            // handshake
		            Log.i(LOGTAG, "Starting handshake (VMF/VMF, (JSON|XML)/OK)");
		            // VMF/VMF
		            sendString("VMF"); 
		            byte[] buff = new byte[3];
		            Log.d(LOGTAG, "Waiting for 'VMF'...");
		            mSockIn.read(buff);
		            String s = new String(buff);
		            Log.d(LOGTAG, "Got " + s);
		            if( !s.equalsIgnoreCase("VMF") )
		            	throw new IOException("Invalid response ('VMF' expected): " + s);
		            // (JSON|XML)/OK
		            if(useXML) sendString("XML");
		            else sendString("JSON");
		            buff = new byte[2];
		            Log.d(LOGTAG, "Waiting for 'OK'");
		            mSockIn.read(buff);
		            s = new String(buff);
		            Log.d(LOGTAG, "Got " + s);
		            if( !s.equalsIgnoreCase("OK") )
		            	throw new IOException("Invalid response ('OK' expected): " + s);

		            Log.i(LOGTAG, "Sending VMF VideoSegment");
	            	String VSegString = mVMFFormat.store(
	            			null,
	            			null, 
            				new MetadataStream.VideoSegment[]{new MetadataStream.VideoSegment("Android", 15, System.currentTimeMillis())},
            				null,
            				null); 
		            sendString(VSegString);

		            Log.i(LOGTAG, "Sending VMF schema");
	            	String StdSchemaString = mVMFFormat.store(
	            			null,
	            			new MetadataSchema[]{MetadataSchema.getStdSchema()},
	            			null,
	            			null,
	            			null);
		            sendString(StdSchemaString);

		            Log.i(LOGTAG, "Ready to stream VMF medatada");
		            mIsVMFStreamming = true;

		            mUseGpsEmulation = useGpsEmu;

		            if(mUseGpsEmulation) {
			            if(mGpsEmulationTread != null) {
			            	mGpsEmulationTread.interrupt();
			            	mGpsEmulationTread = null;
			            }
						mGpsEmulationTread = new Thread(mGpsEmulation);
						mGpsEmulationTread.start();
		            }
				} catch (Exception e) {
					Log.e(LOGTAG, "Error initializing VMF metadata streaming: " + e.getMessage());
		            Log.i(LOGTAG, "STOP streaming fake locations");
					mIsVMFStreamming = false;
					if(mGpsEmulationTread != null) {
		            	mGpsEmulationTread.interrupt();
		            	mGpsEmulationTread = null;
		            }
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
		}};

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
				    	Toast.makeText(MainActivity.this, "STREAMING STARTED", Toast.LENGTH_LONG).show();
				    	mTextURL.setVisibility(View.INVISIBLE);
				    	mTextFormat.setVisibility(View.VISIBLE);
				    	mTextGpsEmu.setVisibility(View.VISIBLE);
				    	mTextCompress.setVisibility(View.VISIBLE);
				    	mTextEncrypt.setVisibility(View.VISIBLE);
				    }
				});

				//assert(mVmfListeningTread == null);
				mVmfListeningTread = new Thread(mVmfListening);
				mVmfListeningTread.start();

			} else if (message==RtspServer.MESSAGE_STREAMING_STOPPED) {
				Log.i(LOGTAG, "MESSAGE_STREAMING_STOPPED");
				final String rtstAddr = "rtsp://" + mIPString + ":" + mRtspServer.getPort();
				runOnUiThread(new Runnable() {
				    public void run() {
				    	Toast.makeText(MainActivity.this, "STREAMING STOPPED", Toast.LENGTH_LONG).show();
				    	mTextURL.setText("Connect " + rtstAddr);
				    	mTextURL.setVisibility(View.VISIBLE);
				    	mTextFormat.setVisibility(View.INVISIBLE);
				    	mTextGpsEmu.setVisibility(View.INVISIBLE);
				    	mTextCompress.setVisibility(View.INVISIBLE);
				    	mTextEncrypt.setVisibility(View.INVISIBLE);

				    }
				});

				Log.i(LOGTAG, "Stop VMF metadata streaming");
				mIsVMFStreamming = false;
				if(mGpsEmulationTread != null) {
	            	mGpsEmulationTread.interrupt();
	            	mGpsEmulationTread = null;
	            }
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
				finally {
					mVmfListeningTread = null;
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
		mSurfaceView = (SurfaceView) findViewById(R.id.surface_preview);
		mTextURL = (TextView) findViewById(R.id.textURL);
		mTextFormat = (TextView) findViewById(R.id.textFormat);
		mTextGpsEmu = (TextView) findViewById(R.id.textGpsEmu);
		mTextCompress = (TextView) findViewById(R.id.textCompress);
		mTextEncrypt = (TextView) findViewById(R.id.textEncrypt);

		// RTSP init

		// Sets the RTSP server port to 1234 if not yet set
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
		if( !prefs.contains(RtspServer.KEY_PORT) ) {
			Editor editor = prefs.edit();
			editor.putString(RtspServer.KEY_PORT, String.valueOf(1234));
			editor.commit();
		}

		SessionBuilder.getInstance()
		        .setSurfaceView(mSurfaceView)
		        .setPreviewOrientation(0)
		        .setContext(getApplicationContext())
		        .setAudioEncoder(SessionBuilder.AUDIO_NONE)
		        .setVideoEncoder(SessionBuilder.VIDEO_H264)
		        .setVideoQuality(new VideoQuality(640,480,15,2000000));

		startService(new Intent(this, RtspServer.class));

		mLocationManager = (LocationManager) this.getSystemService(Context.LOCATION_SERVICE);
    	set.clear();
        set.push_back(m);
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

		mTextURL.setVisibility(View.VISIBLE);
		mTextURL.setText("Connecting to RTSPServer... ");
		bindService(new Intent(this, RtspServer.class), mRtspServiceConnection, Context.BIND_AUTO_CREATE);

		if(!PreferenceManager.getDefaultSharedPreferences(this).getBoolean("gps_emulation", true)) {
			try {
				mLocationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 1000 /*1s*/, 5 /*5m*/, this);
			} catch (Exception e) {
				Log.e(LOGTAG, "Network location service provider not available!");
			}
			mLocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 1000 /*1s*/, 5 /*5m*/, this);
		}
	}

	protected void stopStreaming() {
		if(mRtspServer != null)
			mRtspServer.stop();
		// TODO: sync required

		if(mGpsEmulationTread != null) {
        	mGpsEmulationTread.interrupt();
        	mGpsEmulationTread = null;
        }

		unbindService(mRtspServiceConnection);

		mLocationManager.removeUpdates(this);
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
	public void onLocationChanged(Location loc) {
		lat.setTo(loc.getLatitude());
		lng.setTo(loc.getLongitude());
    	m.setFieldValue("latitude", lat);
    	m.setFieldValue("longitude", lng);
    	m.setTimestamp( System.currentTimeMillis() );
    	if(mIsVMFStreamming && !mUseGpsEmulation) {
	    	try {
				sendString(mVMFFormat.store(set, null, null, null, null));
			} catch (IOException e) {
				Log.e(LOGTAG, "Error sending location:" + e.getMessage());
			}
    	}
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

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        if(e.getAction() == MotionEvent.ACTION_DOWN) {

        	// Preference Screen as a separate Activity
        	//startActivity( new Intent(getApplicationContext(), VmfPreferenceActivity.class) );
        	// or
        	// Preference Screen as a fragment not interrupting the RTSP/VMF sessions
        	getFragmentManager()
			.beginTransaction()
			.replace(android.R.id.content, new VmfPreferenceFragment())
			.addToBackStack(null)
			.commit();
        }
        return super.onTouchEvent(e);
    }
}
