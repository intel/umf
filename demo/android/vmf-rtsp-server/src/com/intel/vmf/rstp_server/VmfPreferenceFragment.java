package com.intel.vmf.rstp_server;

import android.os.Bundle;
import android.preference.PreferenceFragment;

public class VmfPreferenceFragment extends PreferenceFragment {

	@Override
	 public void onCreate(Bundle savedInstanceState) {
	  super.onCreate(savedInstanceState);
	  addPreferencesFromResource(R.xml.pref);
	 } 

}
