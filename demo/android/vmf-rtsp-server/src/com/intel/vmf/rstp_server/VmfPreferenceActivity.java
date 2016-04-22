package com.intel.vmf.rstp_server;

import android.app.Activity;
import android.os.Bundle;

public class VmfPreferenceActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getFragmentManager()
			.beginTransaction()
			.replace(android.R.id.content, new VmfPreferenceFragment())
			.commit();
	}
}
