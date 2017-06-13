package com.example.sockettest001;
 

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.ServerSocket;
import java.net.Socket;


import android.app.Activity;
import android.os.Bundle;
import android.os.StrictMode;
import android.widget.TextView;

public class MainActivity extends Activity {   
    private TextView textview;
    String result;
	public void onCreate(Bundle savedInstanceState) {  
        super.onCreate(savedInstanceState);
        
        StrictMode.setThreadPolicy(new StrictMode.ThreadPolicy.Builder()  
        .detectDiskReads()  
        .detectDiskWrites()  
        .detectNetwork()   // or .detectAll() for all detectable problems  
        .penaltyLog()  
        .build());  
StrictMode.setVmPolicy(new StrictMode.VmPolicy.Builder()  
        .detectLeakedSqlLiteObjects()  
        .detectLeakedClosableObjects()  
        .penaltyLog()  
        .penaltyDeath()  
        .build());
        
        setContentView(R.layout.activity_main);  
        textview=(TextView)findViewById(R.id.textView);
        
        
        
        try {
			int index = 0;
			byte[] receive = new byte[28];
			int k=15;
			while(k>0){
				Socket sock = new Socket("10.2.18.20", 9999);//改成自己的本地主机IP
				
				System.out.println("send content:  name=kingfish  "+"value="+(1+index));
				
				Employee abc=new Employee("kingfish"+index, 1+index++, 10.99f + index);
				sock.getOutputStream().write(abc.getbuf());
				sock.getInputStream().read(receive);
				Employee ee = abc.getEmployee(receive);
				System.out.println("    response:  name="+ee.name+"  "+"value="+ee.id);
				k--;
				sock.close();
			}
			
		} catch (Exception e) {
			e.printStackTrace();
		}
        
    } 
	
	
	
}  