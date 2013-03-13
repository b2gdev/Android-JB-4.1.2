package android.tcbin;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.EnumSet;

import android.app.Activity;
import android.app.KeyguardManager;
import android.app.KeyguardManager.KeyguardLock;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.widget.EditText;
import android.widget.TextView;

import zone24x7.tcbin.TCBINException;
import zone24x7.tcbin.Braille.*;
//import zone24x7.tcbin.Braille.BrailleDisplay.DotStrength;

public class TcbinTextViewActivity extends Activity {

	private static final String STARTUP_MSG_FILE = "Startup.txt";
	private String LOG_SOURCE = "";
	private String startupText = "";
	
	protected BrailleDisplay brailleDisplay = new BrailleDisplay();
	protected RandomAccessFile raf = null;
	protected boolean commandState = false;
	protected TextHandler th = null;
	protected TextView tv = null;
		
	@Override
	public void onCreate(Bundle savedInstanceState) {		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		LOG_SOURCE = getString(R.string.app_name);
		tv = (TextView) findViewById(R.id.tv);
		final EditText et = (EditText) findViewById(R.id.et);
		commandState = false;
		
		et.setBackgroundColor(android.R.color.transparent);
		
		KeyguardManager keyguardManager = (KeyguardManager) getSystemService(Activity.KEYGUARD_SERVICE);
		KeyguardLock lock = keyguardManager.newKeyguardLock(KEYGUARD_SERVICE);
		lock.disableKeyguard();

		// show Welcome message
		showWelcome();
		
		// check the SD card
		checkSD();

		// open file in SD card
		th = new TextHandler("/default.txt");

		// Key event are handled here
		et.setOnKeyListener(new OnKeyListener() {
			private boolean isHandled = false;
			private boolean commandPressed = false;

			public boolean onKey(View v, int keyCode, KeyEvent event) {
				Log.d(LOG_SOURCE, Integer.toString(keyCode));
				
				if ((event.getAction() == KeyEvent.ACTION_UP)) {					 
					 if ((keyCode == BrailleKeyEvent.KEYCODE_BRL_DOT4)) {
						 commandPressed = false;
					 }
				}
				else if ((event.getAction() == KeyEvent.ACTION_DOWN)) {
					if (commandState == false) {
						switch (keyCode) {
						
						case (BrailleKeyEvent.KEYCODE_BRL_FORWARD):
						case (KeyEvent.KEYCODE_DPAD_DOWN):
							th.nextLine();
							isHandled = true;
							break;
							
						case (BrailleKeyEvent.KEYCODE_BRL_BACK):
						case (KeyEvent.KEYCODE_DPAD_UP):
							th.prevLine();
							isHandled = true;
							break;
							
						case (KeyEvent.KEYCODE_BACK):
						case (KeyEvent.KEYCODE_MENU):
							isHandled = false;
							break;

						case (BrailleKeyEvent.KEYCODE_BRL_DOT4):
							commandPressed = true;
							isHandled = true;
							break;

						case (KeyEvent.KEYCODE_DPAD_LEFT):
							if (commandPressed == true) {
								Log.d(LOG_SOURCE,
										"Command Pressed, going to command mode");
								commandModeOn();
							}
							isHandled = true;
							break;

						default:
							isHandled = true;
							break;
						}

					}
					else {
						switch (keyCode) {
						case BrailleKeyEvent.KEYCODE_BRL_1:
							newFopen(1);
							break;

						case BrailleKeyEvent.KEYCODE_BRL_2:
							newFopen(2);
							break;

						case BrailleKeyEvent.KEYCODE_BRL_3:
							newFopen(3);
							break;

						case BrailleKeyEvent.KEYCODE_BRL_4:
							newFopen(4);
							break;

						case BrailleKeyEvent.KEYCODE_BRL_5:
							newFopen(5);
							break;
						
						case (KeyEvent.KEYCODE_BACK):
						case (KeyEvent.KEYCODE_MENU):
							isHandled = false;
							break;

						case (BrailleKeyEvent.KEYCODE_BRL_DOT4):
							commandPressed = true;
							isHandled = true;
							break;

						case (KeyEvent.KEYCODE_DPAD_LEFT):
							if (commandPressed == true) {
								Log.d(LOG_SOURCE, "Command Pressed, going back to reader mode");
								th.sameLine();
								commandState = false;
								isHandled = true;
							}
							isHandled = true;
							break;

						default:
							isHandled = true;
							commandState = true;
							break;
						}	//switch (keyCode)
					}	//commandState == true
				}	//Action == KeyEvent.ACTION_DOWN

				return isHandled;
			}

			private void newFopen(int fileNo) {
				String s = new String("/Braille" + Integer.toString(fileNo)
						+ ".txt");
				TextHandler thOld = th;
				th = new TextHandler(s);
				isHandled = true;
				if (th.br != null) {
					commandState = false;
					th.nextLine();
				} else {
					th = thOld;
				}
			}
		});
	}

	/*
	 * method to test if the SD card is mounted. The App will not load if SD
	 * card is not present
	 */
	public static boolean isSdPresent() {
		return android.os.Environment.getExternalStorageState().equals(
				android.os.Environment.MEDIA_MOUNTED);
	}
	
	/*
	 * method to test if the SD card is mounted. The App will not load if SD
	 * card is not present
	 */
	private void checkSD() {
		long startTime = System.currentTimeMillis();
		long curTime = 0;

		while (isSdPresent() == false) {
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {}

			curTime = System.currentTimeMillis();
			Log.d(LOG_SOURCE, Long.toString((curTime - startTime)));

			if ((curTime - startTime) > 10000) {				
				try {
					brailleDisplay.displayText(getString(R.string.no_sdcard_msg));
				} catch (TCBINException e) {
					Log.d(LOG_SOURCE, e.getMessage());
				}
				
				Log.d(LOG_SOURCE, "Going to finish");
				System.exit(RESULT_CANCELED);
				Log.d(LOG_SOURCE, "Invoke Finish()");
			}
		}
	}

	/*show the Welcome message*/
	private void showWelcome() {		
		BufferedReader reader = null;		 
		try {
			reader = new BufferedReader(new FileReader(new File(Environment.getExternalStorageDirectory(), 
																STARTUP_MSG_FILE)));
			startupText = reader.readLine();	
			if (startupText.length() > brailleDisplay.getColumns()) {
				startupText = startupText.substring(0, brailleDisplay.getColumns() - 1);
			}
			brailleDisplay.displayText(startupText);
			tv.setText(startupText);						
		} catch (FileNotFoundException e) {
			Log.d(LOG_SOURCE, STARTUP_MSG_FILE + " not found.");
		} catch (IOException e) {
			Log.d(LOG_SOURCE, "Failed to read from " + STARTUP_MSG_FILE);
		} catch (TCBINException e) {
			Log.d(LOG_SOURCE, e.getMessage());
		} finally {
			try {
				if (reader != null) {
					reader.close();
				}	
			} catch (IOException e) {}
		}		
	}

	private void commandModeOn() {
		try {
			String text = getString(R.string.cmd_mode_text);
			brailleDisplay.displayText(text);
			tv.setText(text);			
		} catch (TCBINException e) {
			Log.d("BRAILLE", e.getMessage());
		}
		commandState = true;
	}

	/*Helper class to process the text file*/
	private class TextHandler {

		private RandomAccessFile br;
		private String readString;
		private int lineNo;

		public TextHandler(String fn) {
			String fname = fn;
			File myFile;
			RandomAccessFile rf;
			readString = startupText;

			try {
				myFile = new File(Environment.getExternalStorageDirectory(),
						fname);
				rf = new RandomAccessFile(myFile, "r");

			} catch (FileNotFoundException e) {
				e.printStackTrace();
				rf = null;
				try {
					brailleDisplay.displayText("File doesn't exist");
				} catch (TCBINException e1) {
					Log.d(LOG_SOURCE, e.getMessage());
				}
				tv.setText("File doesn't exist");

			}

			if (rf != null) {
				this.br = rf;
				this.lineNo = 1;
				Log.d(LOG_SOURCE, "File opened");
			}

		}

		private void nextLine() {

			try {
				Log.d(LOG_SOURCE, "NextLine" + lineNo + br.getFilePointer());

				if ((readString = br.readLine()) != null) {
					lineNo += 1;
					try {
						brailleDisplay.displayText(readString);
						
					} catch (TCBINException e) {
						Log.d(LOG_SOURCE, e.getMessage());
					}
					tv.setText(readString);

				}

			} catch (IOException e) {
				e.printStackTrace();
			}

		}// nextlin

		private void prevLine() {

			byte[] ba = new byte[1];
			int nlc = 0;

			try {
				Log.d(LOG_SOURCE, "PrevLine" + lineNo + br.getFilePointer());

				while ((nlc != 2) && (br.getFilePointer() != 1)) {
					br.seek(br.getFilePointer() - 2);
					ba[0] = br.readByte();
					if (ba[0] == 0x0A) {
						nlc++;
						lineNo--;
					}

				}

				if (br.getFilePointer() == 1)
					br.seek(0);
				if ((readString = br.readLine()) != null) {
					try {
						brailleDisplay.displayText(readString);
					} catch (TCBINException e) {
						Log.d(LOG_SOURCE, e.getMessage());
					}
					tv.setText(readString);

				}

			} catch (IOException e) {
				Log.d(LOG_SOURCE, "Failed to access file.");
			}

		}// prevline

		private void sameLine() {
			try {
				brailleDisplay.displayText(readString);
				tv.setText(readString);				
			} catch (TCBINException e) {
				Log.d(LOG_SOURCE, e.getMessage());
			}
		}

	}// texthandler

}
