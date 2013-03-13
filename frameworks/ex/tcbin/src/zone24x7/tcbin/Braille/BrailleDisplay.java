package zone24x7.tcbin.Braille;

import zone24x7.tcbin.TCBINException;


public class BrailleDisplay {
	
	public enum DotStrength {
		Level1,
		Level2,
		Level3,
		Level4,
		Level5,
		Level6,
		Level7,
		Level8		
	}
	
	private static final String NATIVE_LIB_NAME = "brailledisplay";
	private int displayRows = 0;
	private int displayColumns = 0;
		
	private static final int UOUT1_ENABLE = (1 << 4);
	private static final int UOUT2_ENABLE = (1 << 5);
	private static final int UOUT3_ENABLE = (1 << 7);
	
	private static final int UOUT_155V_CONFIG_VALUE	= 0;
	private static final int UOUT_162V_CONFIG_VALUE	= (UOUT1_ENABLE);
	private static final int UOUT_168V_CONFIG_VALUE	= (UOUT2_ENABLE);
	private static final int UOUT_174V_CONFIG_VALUE	= (UOUT2_ENABLE | UOUT1_ENABLE);
	private static final int UOUT_177V_CONFIG_VALUE = (UOUT3_ENABLE);
	private static final int UOUT_184V_CONFIG_VALUE	= (UOUT3_ENABLE | UOUT1_ENABLE);
	private static final int UOUT_191V_CONFIG_VALUE	= (UOUT3_ENABLE | UOUT2_ENABLE);
	private static final int UOUT_199V_CONFIG_VALUE	= (UOUT3_ENABLE | UOUT2_ENABLE | UOUT1_ENABLE);	

    static {  
        System.loadLibrary(NATIVE_LIB_NAME);  
    }   		
	
	public BrailleDisplay(){	
		try {
			displayColumns = getColumns();
			displayRows = getRows();			
		} catch (TCBINException e) {}		
	}
		
	public void displayText(String text) throws TCBINException{
		if (text.length() <= displayColumns){
			int result = nativeDisplayText(text);
			if (result != 0) {
				throw new TCBINException("Braille Display - display text : Failed.");
			}
		} else {
			throw new TCBINException("Braille Display - Text length exceeds display size.");
		}
	}
	
	public void clear() throws TCBINException{
			int result = nativeClear();
			if (result != 0) {
				throw new TCBINException("Braille Display - clear : Failed.");
			}
	}
	
	public void setDotStrength(DotStrength value) throws TCBINException{
		int result = nativeSetDotStrength(convertToDotStrength(value));
		if (result != 0) {
			throw new TCBINException("BrailleDisplay - set dot strength : Failed.");
		}
	}
	
	public int getRows() throws TCBINException{
		return nativeGetRows();
	}
	
	public int getColumns() throws TCBINException{
		return nativeGetColumns();
	}
	
	private int convertToDotStrength(DotStrength value) throws TCBINException{
		int dotStrength = 0;
		switch (value) {
			case Level1:
				dotStrength = UOUT_155V_CONFIG_VALUE;
				break;
			case Level2:
				dotStrength = UOUT_162V_CONFIG_VALUE;
				break;
			case Level3:
				dotStrength = UOUT_168V_CONFIG_VALUE;
				break;
			case Level4:
				dotStrength = UOUT_174V_CONFIG_VALUE;
				break;
			case Level5:
				dotStrength = UOUT_177V_CONFIG_VALUE;
				break;
			case Level6:
				dotStrength = UOUT_184V_CONFIG_VALUE;
				break;
			case Level7:
				dotStrength = UOUT_191V_CONFIG_VALUE;
				break;
			case Level8:
				dotStrength = UOUT_199V_CONFIG_VALUE;
				break;
			default:
				throw new TCBINException("Invalid dot strength value.");
		}
		return dotStrength;
	}
	
	private native int nativeDisplayText(String text);
	private native int nativeClear();
	private native int nativeSetDotStrength(int value); 
	private native int nativeGetRows();
	private native int nativeGetColumns();
}
