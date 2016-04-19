package com.intel.vmf;

public class EncryptorDefault extends Encryptor {

	protected EncryptorDefault(long addr) {
		super(addr);
	}
	
	public EncryptorDefault(String passphrase) {
		super( n_EncryptorDefault(passphrase) );
	}

	@Override
	public Variant encrypt(String text) {
		if(text == null) return null;
		long addr = n_encrypt(nativeObj, text);
		if(addr == 0) return null;
		else return new Variant(addr);
	}

	@Override
	public String decrypt(Variant data) {
		if(data == null) return null;
		return n_decrypt(nativeObj, data.nativeObj);
	}

	@Override
	public String getHint() {
		return n_getHint(nativeObj);
	}

	private native static long n_EncryptorDefault(String passphrase);
	private native static long n_encrypt(long self, String text);
	private native static String n_decrypt(long self, long variantAddr);
	private native static String n_getHint(long self);
}
