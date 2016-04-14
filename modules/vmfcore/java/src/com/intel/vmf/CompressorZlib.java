package com.intel.vmf;

public class CompressorZlib extends Compressor {

	public CompressorZlib(long addr) {
		super(addr);
	}

	@Override
	public Variant compress(String input) {
		long addr = n_compress(nativeObj, input);
		if(addr == 0) return null;
		else return new Variant(addr);
	}

	@Override
	public String decompress(Variant data) {
		return n_decompress(nativeObj, data.nativeObj);
	}

	@Override
	public Compressor createNewInstance() {
		long addr = n_createNewInstance(nativeObj);
		if(addr == 0) return null;
		else return new CompressorZlib(addr);
	}

	@Override
	public String getId() {
		//return n_getId(nativeObj);
		return Compressor.BUILTIN_ZLIB;
	}
	
	private static native long n_compress(long self, String str);
	private static native String n_decompress(long self, long variantAddr);
	private static native long n_createNewInstance(long self);

}
