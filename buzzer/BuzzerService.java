
/*   
 *  Buzzer de java code
 */

 package com.android.server;
 import android.os.IBuzzerService;
 import android.util.Slog;

 public class BuzzerService extends IBuzzerService.Stub{
	
	private static final String TAG = "buzzerservice";
	private int mPtr = 0;
	public native static int native_buzzerOpen();
	public native static int native_buzzerCtrl(int cmd,long frec);

	public int buzzerCtrl(int cmd, long frec) throws android.os.RemoteException{
		if(0 == mPtr){
			Slog.e(TAG, "buzzer service is not open");
			return 0;
		}
		return native_buzzerCtrl(cmd,frec);
	}
	public BuzzerService(){
		Slog.i(TAG,"init buzzer device in buzzerService");
		mPtr = native_buzzerOpen();
	}
 }


