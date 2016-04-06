package com.intel.vmf.rstp_server;

import net.majorkernelpanic.streaming.SessionBuilder;
import net.majorkernelpanic.streaming.gl.SurfaceView;
import net.majorkernelpanic.streaming.rtsp.RtspServer;
import net.majorkernelpanic.streaming.video.VideoQuality;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;

public class MainActivity extends Activity {

    protected SurfaceView mSurfaceView;

	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);
		//getWindow().setFormat(PixelFormat.TRANSLUCENT);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		setContentView(R.layout.activity_main);
		mSurfaceView = (SurfaceView) findViewById(R.id.surface_preview);		
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
}
