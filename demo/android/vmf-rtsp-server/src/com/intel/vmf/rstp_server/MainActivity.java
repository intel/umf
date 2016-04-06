package com.intel.vmf.rstp_server;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Collections;
import java.util.List;

import net.majorkernelpanic.streaming.SessionBuilder;
import net.majorkernelpanic.streaming.gl.SurfaceView;
import net.majorkernelpanic.streaming.rtsp.RtspServer;
import net.majorkernelpanic.streaming.rtsp.RtspServer.CallbackListener;
import net.majorkernelpanic.streaming.video.VideoQuality;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.location.Location;
import android.location.LocationListener;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

public class MainActivity extends Activity implements  LocationListener {

	public static final String LOGTAG = "MainActivity";
    
	protected SurfaceView mSurfaceView;
	protected String mIPString = "unknown";

	private ServiceConnection mRtspServiceConnection = new ServiceConnection() {
		
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			Log.d(LOGTAG, "onServiceConnected");
			mRtspServer = ((RtspServer.LocalBinder)service).getService();
			mRtspServer.addCallbackListener(mRtspCallbackListener);
			mRtspServer.start();
			final String addr = "rtsp://" + mIPString + ":" + mRtspServer.getPort();
			Log.i(LOGTAG, "Ready for connection via " + addr);
			runOnUiThread(new Runnable() {
			    public void run() {
			    	//tvIP.setText("Connect " + addr);
					//tvIP.setVisibility(View.VISIBLE);
			    	Toast.makeText(MainActivity.this, addr, Toast.LENGTH_LONG).show();
			    }
			});
		}

		@Override
		public void onServiceDisconnected(ComponentName name) {
			Log.d(LOGTAG, "onServiceDisconnected");
		}
	};

	protected RtspServer mRtspServer;
	
	private RtspServer.CallbackListener mRtspCallbackListener = new RtspServer.CallbackListener() {

		@Override
		public void onError(RtspServer server, Exception e, int error) {
			if (error == RtspServer.ERROR_BIND_FAILED) {
				runOnUiThread(new Runnable() {
				    public void run() {
				    	Toast.makeText(MainActivity.this, "The selected RSTP port is already in use!", Toast.LENGTH_LONG).show();
				    }
				});
			}
		}

		@Override
		public void onMessage(RtspServer server, int message) {
			if (message==RtspServer.MESSAGE_STREAMING_STARTED) {
				runOnUiThread(new Runnable() {
				    public void run() {
						//tvIP.setVisibility(View.INVISIBLE);
				    	Toast.makeText(MainActivity.this, "STREAMING STARTED", Toast.LENGTH_LONG).show();
				    }
				});
				/*
				new Thread("Streaming VMF Metadata") {
					public void run() {
			            Log.w(LOGTAG, "Starting streaming VMF metadata");

			    		int VmfPort = Integer.parseInt(
			    				PreferenceManager.getDefaultSharedPreferences(getApplicationContext())
			    					.getString("vmfPort", "4321") );

						try {
							mServerSocket = new ServerSocket(VmfPort);
				            mSocket = mServerSocket.accept();
				            Log.d(LOGTAG, "Connection accepted!");
				            mDOS = new DataOutputStream(mSocket.getOutputStream());
				            DataInputStream  dis = new DataInputStream(mSocket.getInputStream());

				            // handshake

				            Log.d(LOGTAG, "Starting handshake (VMF/VMF, XML/OK)");
				            sendStr("VMF");
				            byte[] buff = new byte[3];
				            Log.d(LOGTAG, "Waiting for 'VMF'...");
				            dis.read(buff); // dis.readUTF();
				            String s = new String(buff);
				            if( !s.equalsIgnoreCase("VMF") )
				            	throw new IOException("Invalid response ('VMF' expected): " + s);

				            sendStr("XML");
				            buff = new byte[2];
				            Log.d(LOGTAG, "Waiting for 'OK'");
				            dis.read(buff); // dis.readUTF();
				            s = new String(buff);// dis.readUTF();
				            if( !s.equalsIgnoreCase("OK") )
				            	throw new IOException("Invalid response ('OK' expected): " + s);

				            Log.d(LOGTAG, "Sending VMF schema");
				            String schema = getSchemaString(mVmfMdStream);
				            sendStr(schema);

				            Log.d(LOGTAG, "Activate metadata streaming");
				            mStartedStreamingVMF = true;
						} catch (Exception e) {
							Log.e(LOGTAG, "Error streaming VMF metadata: " + e.getMessage());
			        	}
				}}.start();*/
			} else if (message==RtspServer.MESSAGE_STREAMING_STOPPED) {
				runOnUiThread(new Runnable() {
				    public void run() {
						//tvIP.setVisibility(View.VISIBLE);
				    	Toast.makeText(MainActivity.this, "STREAMING STOPPED", Toast.LENGTH_LONG).show();
				    }
				});

				/*
				Log.d(LOGTAG, "Stop metadata streaming");
				mDOS = null;
				try {
					if(mSocket != null) {
						mSocket.close();
						mSocket = null;
					}
					mServerSocket.close();
					mServerSocket = null;
				} catch (IOException e) {
					Log.e(LOGTAG, "Error closing sockets: " + e.getMessage());
				}
				*/
			}
		}

	};


	@Override
    protected void onCreate(Bundle savedInstanceState) {
		Log.d(LOGTAG, "onCreate");
        super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);
		//getWindow().setFormat(PixelFormat.TRANSLUCENT);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		setContentView(R.layout.activity_main);
		mSurfaceView = (SurfaceView) findViewById(R.id.surface_preview);
		//mSurfaceView.getHolder().addCallback(this);

		// RTSP init
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
		//stopStreaming();
		stopService(new Intent(this, RtspServer.class));
		super.onDestroy();
	}

	@Override
	public void onPause() {
		Log.d(LOGTAG, "onPause");
		stopStreaming();
		//mLocationManager.removeUpdates(this);
		super.onPause();
	}

	@Override
	public void onResume() {
		Log.d(LOGTAG, "onResume");
		super.onResume();
		startStreaming();
		//mLocationManager...
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

		bindService(new Intent(this, RtspServer.class), mRtspServiceConnection, Context.BIND_AUTO_CREATE);
	}

	protected void stopStreaming() {
		if(mRtspServer != null)
			mRtspServer.stop();
		// TODO: sync required
		unbindService(mRtspServiceConnection);
		mRtspServer = null;

		// TODO: socket
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
