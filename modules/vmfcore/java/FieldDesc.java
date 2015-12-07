
public class FieldDesc
{
    protected final long nativeObj;
	
	public boolean optional;
	
	public String name;
	
	Variant.Type type;
	
	public FieldDesc ()
	{
        nativeObj = n_FieldDesc();
	}
	
	public boolean isEquals()
	{
        return n_isEquals();
	}
	
	private native long n_FieldDesc ();
	
	private native boolean n_isEquals();
}