package com.shankar92gmail.anush.bluetooth_connect_tofixed_mac;


        import java.io.IOException;
        import java.io.InputStream;
        import java.io.OutputStream;
        import java.lang.reflect.Method;
        import java.util.UUID;


        import android.app.Activity;
        import android.bluetooth.BluetoothAdapter;
        import android.bluetooth.BluetoothDevice;
        import android.bluetooth.BluetoothSocket;
        import android.content.Intent;
        import android.os.Build;
        import android.os.Bundle;
        import android.util.Log;
        import android.view.View;
        import android.view.View.OnClickListener;
        import android.widget.Button;
        import android.widget.Toast;

public class MainActivity extends Activity {
    private static final String TAG = "bluetooth1";

    Button btnOn, btnOff,btOn,btConnect,btdisconnect,sndrt;
    int BT_CONNECT_CODE = 1;
    int connect = 0;
    private BluetoothAdapter btAdapter;
    private BluetoothSocket btSocket;
    private OutputStream outStream = null;
    private InputStream instream = null;

    // SPP UUID service
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    // MAC-address of Bluetooth module (you must edit this line)
    private static String address = "20:15:03:03:09:75";

    private static String tx_data1 = "10000000\n";

    private static String tx_data2 = "00000000\n";

    private static  String send_data = "37.33433 \n";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        btnOn = (Button) findViewById(R.id.btnOn);
        btnOff = (Button) findViewById(R.id.btnOff);
        btOn = (Button) findViewById(R.id.btOn);
        btConnect = (Button) findViewById(R.id.btConnect);
        btdisconnect = (Button)findViewById(R.id.btdisconnect);
        sndrt = (Button)findViewById(R.id.sndrt);

        btAdapter = BluetoothAdapter.getDefaultAdapter();
        btOn.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                checkBTState();
            }
        });

        btnOn.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                if(connect == 1) {
                    sendData(tx_data1);
                    Toast.makeText(getBaseContext(), "Turn on LED", Toast.LENGTH_SHORT).show();
                }
            }
        });

        btnOff.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                if(connect == 1) {
                    sendData(tx_data2);
                    Toast.makeText(getBaseContext(), "Turn off LED", Toast.LENGTH_SHORT).show();
                }
            }
        });

        btConnect.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                if (connect == 0) {
                    connect();
                }
            }
        });

        btdisconnect.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                if(connect == 1) {
                    disconnect();
                }
            }
        });

        sndrt.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                if (connect == 1) {
                    sendData(send_data);
                    Toast.makeText(getBaseContext(), "Turn off LED", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {
        if(Build.VERSION.SDK_INT >= 10){
            try {
                final Method  m = device.getClass().getMethod("createInsecureRfcommSocketToServiceRecord", new Class[] { UUID.class });
                return (BluetoothSocket) m.invoke(device, MY_UUID);
            } catch (Exception e) {
                Log.e(TAG, "Could not create Insecure RFComm Connection",e);
            }
        }
        return  device.createRfcommSocketToServiceRecord(MY_UUID);
    }


    private void checkBTState() {
        // Check for Bluetooth support and then check to make sure it is turned on
        // Emulator doesn't support Bluetooth and will return null
        if(btAdapter==null) {
            errorExit("Fatal Error", "Bluetooth not support");
        } else {
            if (btAdapter.isEnabled()) {
                Log.d(TAG, "...Bluetooth ON...");
            } else {
                //Prompt user to turn on Bluetooth
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, BT_CONNECT_CODE);
            }
        }
    }

    private void errorExit(String title, String message){
        Toast.makeText(getBaseContext(), title + " - " + message, Toast.LENGTH_LONG).show();
        finish();
    }

    private void sendData(String message) {
        byte[] msgBuffer = message.getBytes();

        Log.d(TAG, "...Send data: " + message + "...");

        try {
            outStream.write(msgBuffer);
            Toast.makeText(getApplicationContext(),"data sent",Toast.LENGTH_LONG).show();
        } catch (IOException e) {
            String msg = "In onResume() and an exception occurred during write: " + e.getMessage();
            if (address.equals("00:00:00:00:00:00"))
                msg = msg + ".\n\nUpdate your server address from 00:00:00:00:00:00 to the correct address on line 35 in the java code";
            msg = msg +  ".\n\nCheck that the SPP UUID: " + MY_UUID.toString() + " exists on server.\n\n";

            errorExit("Fatal Error", msg);
        }
    }

    private void disconnect() {
        try{
            instream = btSocket.getInputStream();
        }catch (IOException e){
            errorExit("Fatal Error", "In onResume() and input stream creation failed:" + e.getMessage() + ".");
        }

        if (instream != null) {
            try {instream.close();} catch (Exception e) {}
            instream = null;
        }

        if (outStream != null) {
            try {outStream.close();} catch (Exception e) {}
            outStream = null;
        }

        if (btSocket != null) {
            try {btSocket.close();
                Toast.makeText(getApplicationContext(), "Disconnected", Toast.LENGTH_SHORT).show();
                connect = 0;
            } catch (Exception e) {}
            btSocket = null;
        }

    }

    private void connect() {
        // Set up a pointer to the remote node using it's address.
        BluetoothDevice device = btAdapter.getRemoteDevice(address);

            try {
                btSocket = createBluetoothSocket(device);
            } catch (IOException e1) {
                errorExit("Fatal Error", "In onResume() and socket create failed: " + e1.getMessage() + ".");
            }

            btAdapter.cancelDiscovery();

            // Establish the connection.  This will block until it connects.
            Log.d(TAG, "...Connecting...");
            try {
                btSocket.connect();
                Log.d(TAG, "...Connection ok...");
                connect = 1;
                Toast.makeText(getApplicationContext(), "Connected", Toast.LENGTH_SHORT).show();

            } catch (IOException e) {
                try {
                    btSocket.close();
                } catch (IOException e2) {
                    errorExit("Fatal Error", "In onResume() and unable to close socket during connection failure" + e2.getMessage() + ".");
                }
            }

            Log.d(TAG, "...Create Socket...");

            try {
                outStream = btSocket.getOutputStream();
            } catch (IOException e) {
                errorExit("Fatal Error", "In onResume() and output stream creation failed:" + e.getMessage() + ".");

            }
    }

    public void onActivityResult(int req_code, int res_code, Intent data)
    {
        if(req_code == BT_CONNECT_CODE)
        {
            if(res_code == RESULT_OK)
            {
                Toast.makeText(getBaseContext(),"BLUETOOTH SUCCESSFULLY ENABLED", Toast.LENGTH_LONG).show();

            }
            if(res_code == RESULT_CANCELED)
            {
                Toast.makeText(getBaseContext(),"BLUETOOTH TURN ON FAILED", Toast.LENGTH_LONG).show();
            }
        }
    }

}



