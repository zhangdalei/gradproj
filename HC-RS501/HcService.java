/* HcService for graduation project  */

package com.android.server;
import android.os.IHcService;
import android.util.Slog;

public class HcService extends IHcService.Stub{
   
	private static final String TAG = "Hcservice";
	private int mPtr = 0;
	public native static int native_hcOpen();
	public native static int native_hcRead();	

	public int HcRead(){
		if( 0 == mPtr){
			Slog.e(TAG, "hc501 service is not open");
			return 0;
		}
		return native_hcRead();
	}

	public HcService(){
	   Slog.i(TAG,"init Hc501 device in hcService");
	   mPtr = native_hcOpen();
	}
}




