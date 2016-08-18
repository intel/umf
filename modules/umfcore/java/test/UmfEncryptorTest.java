import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import org.junit.Before;
import org.junit.Test;

import com.intel.umf.Encryptor;
import com.intel.umf.EncryptorDefault;
import com.intel.umf.Variant;


public class UmfEncryptorTest {

	@Before
    public void setUp() {
    }

    @Test
    public void testEncryptor () {
    	String passphrase = "This is my secret passphrase";
    	Encryptor enc = new EncryptorDefault(passphrase);
        assertNotNull(enc);
        
        String encryptorDefaultHint = "Password-Based decryptor using TripleDES and HMAC/SHA-1";
        assertTrue(enc.getHint().equals(encryptorDefaultHint));
        
    	Encryptor enc2 = new EncryptorDefault(passphrase);
        assertNotNull(enc2);
        assertTrue(enc2.getHint().equals(enc.getHint()));
        
        String text = "This is a text for Encryptor test";
        Variant v = enc.encrypt(text);
        assertNotNull(v);
        String text2 = enc2.decrypt(v);
        assertTrue(text.equals(text2));
    }
}
