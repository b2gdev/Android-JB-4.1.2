package android.tcbin;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class MyBroadcastReceiver extends BroadcastReceiver {

	static boolean didRun = false;

	@Override
	public void onReceive(Context context, Intent intent) {
		Intent startServiceIntent = new Intent(context,
				TcbinTextViewActivity.class);
		Log.d("BRAILE_READER", " BrailReader_start_received");

		/*start the BrailReader Application*/
		startServiceIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);

		context.startActivity(startServiceIntent);
		Log.d("BRAILE_READER", " BrailReader_started");

	}

}

