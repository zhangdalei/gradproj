package bbk.usbcamera;

import android.app.Activity;
import android.os.Bundle;
import android.os.IBuzzerService;
import android.os.IHcService;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity implements View.OnClickListener ,Runnable{

    private CameraPreview cp;
    private static final String TAG = "hc";
    private Button buttonOn = null;
    private Button buttonOff = null;
    private Button buttonAdd = null;
    private Button buttonDesc = null;
    private TextView textViewFrec = null;
    private IBuzzerService buzzerService = null;
    private IHcService hcService = null;
    private static final int BUZZER_ON = 1;
    private static final int BUZZER_OFF = 0;
    private static long frecq = 2000;
    private volatile boolean hcexit = false;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);
        // 显示图片的surfaceview
        cp = (CameraPreview) findViewById(R.id.cp);
        initView();
        Thread buzzerThread = new Thread(this);
        buzzerThread.start();
    }

    /**
     *  根据热释电的值,控制蜂鸣器，并修改标志值，以保存摄像头图片
     *  推出时通过标志值来销毁该进程
     */
    @Override
    public void run() {
        while(!hcexit) {
            int data = 0;
            try {
                data = hcService.HcRead();
                if(hcexit){
                    Log.d(TAG, "distory hcbuzzer thread");
                    break;
                }
                //Log.d(TAG, "read data in app is " + data);
                if (data == 1) {
                    CameraPreview.exit = false;
                    buzzerService.buzzerCtrl(BUZZER_ON, frecq);
                } else if (data == 0) {
                    CameraPreview.exit = true;
                    buzzerService.buzzerCtrl(BUZZER_OFF, frecq);
                }
            } catch (RemoteException e) {
                Log.d(TAG, e.toString());
            } catch (Exception e) {
                CameraPreview.exit = true;
                e.printStackTrace();
            }
        }
    }

    @Override
    public void onClick(View view) {
        //String frec = textViewFrec.getText().toString();
        frecq = Long.parseLong(textViewFrec.getText().toString());
        //Log.d(TAG, "buzzer frec:"+ frecq);
        switch (view.getId()){
            case R.id.on:
                try{
                    buzzerService.buzzerCtrl(BUZZER_ON, frecq);
                }catch (RemoteException e){
                    e.printStackTrace();
                }
                break;
            case R.id.off:
                try{
                    buzzerService.buzzerCtrl(BUZZER_OFF,frecq);
                }catch (RemoteException e){
                    e.printStackTrace();
                }
                break;
            case R.id.add_frec:
                try{
                    frecq += 100;
                    buzzerService.buzzerCtrl(BUZZER_ON,frecq);
                    textViewFrec.setText(Long.toString(frecq));
                }catch (RemoteException e) {
                    e.printStackTrace();
                }
                break;
            case R.id.desc_frec:
                try{
                    frecq -= 100;
                    buzzerService.buzzerCtrl(BUZZER_ON,frecq);
                    textViewFrec.setText(Long.toString(frecq));
                }catch (RemoteException e) {
                    e.printStackTrace();
                }
        }
    }

    private void initView() {
        buttonOn = (Button) findViewById(R.id.on);
        buttonOff = (Button) findViewById(R.id.off);
        buttonAdd = (Button) findViewById(R.id.add_frec);
        buttonDesc = (Button) findViewById(R.id.desc_frec);
        textViewFrec = (TextView) findViewById(R.id.frec);
        buzzerService = IBuzzerService.Stub.asInterface(ServiceManager.getService("buzzer"));
        hcService = IHcService.Stub.asInterface(ServiceManager.getService("hc"));
        buttonOn.setOnClickListener(this);
        buttonOff.setOnClickListener(this);
        buttonAdd.setOnClickListener(this);
        buttonDesc.setOnClickListener(this);
    }

    @Override
    protected void onPause() {
        hcexit = true;
        CameraPreview.exit = true;
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        try{
            Log.d(TAG ,"distory MainActivity");
            buzzerService.buzzerCtrl(BUZZER_OFF,frecq);
            hcService = null;
            buzzerService = null;
        }catch (RemoteException e){
            e.printStackTrace();
        }
        super.onDestroy();
    }

}
