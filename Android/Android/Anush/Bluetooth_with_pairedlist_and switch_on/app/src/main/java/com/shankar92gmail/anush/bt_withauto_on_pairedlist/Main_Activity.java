package com.shankar92gmail.anush.bt_withauto_on_pairedlist;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.util.Set;

public class Main_Activity extends Activity {

    Button listpairbtn;
    Button onbutton;
    Button discover_button;
    BluetoothAdapter btadapter;
    Set<BluetoothDevice> paired_devices;
    String plist[];
    BluetoothAdapter discadapter;
    Set<BluetoothDevice> disc_devices;
    String disclist[];
    int k = 0;
    String action;
    IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_);
        listpairbtn = (Button) findViewById(R.id.button_listpaired);
        onbutton = (Button) findViewById(R.id.button);
        discover_button = (Button)findViewById(R.id.button2);
        btadapter = BluetoothAdapter.getDefaultAdapter();

        onbutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if (btadapter == null) {
                    Toast.makeText(getApplicationContext(), "NO BLUETOOTH SUPPORT ON DEVICE", Toast.LENGTH_LONG).show();
                }

                else if (!btadapter.isEnabled()) {
                        Intent discoverintent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
                        discoverintent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
                        startActivity(discoverintent);
                        Toast.makeText(getApplicationContext(), "TURNING ON BLUETOOTH AND MAKING IT VISIBLE", Toast.LENGTH_LONG).show();

                }
            }
        });

        listpairbtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!btadapter.isEnabled()) {
                    Toast.makeText(getApplicationContext(), "BLUETOOTH NOT TURNED ON, TURN ON TO CONTINUE", Toast.LENGTH_LONG).show();
                }

                else if (btadapter.isEnabled()) {
                    paired_devices = btadapter.getBondedDevices();
                    int count = paired_devices.size();
                    plist = new String[count * 2];
                    int j = 0;
                    for (BluetoothDevice device : paired_devices) {

                        plist[j] = device.getName().toString();
                        plist[j + 1] = device.getAddress().toString();
                        j = j + 2;
                    }
                    Bundle bn = new Bundle();
                    bn.putStringArray("paires", plist);
                    Intent in = new Intent("pair_filter");
                    in.putExtras(bn);
                    startActivity(in);
                }
            }
        });

        discover_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                btadapter.cancelDiscovery();
                btadapter.startDiscovery();
                disclist = new String[20];
                Toast.makeText(getApplicationContext(), "DISCOVERY STARTED", Toast.LENGTH_LONG).show();
            }
        });
        registerReceiver(mReceiver, filter);
        IntentFilter filter_disc = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        registerReceiver(mReceiver, filter_disc);

    }

    BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            action = intent.getAction();
            // When discovery finds a device
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                // Get the BluetoothDevice object from the Intent
                Toast.makeText(getApplicationContext(), "DEVICE FOUND", Toast.LENGTH_LONG).show();

                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                disclist[k] = device.getName().toString();
                disclist[k + 1] = device.getAddress().toString();
                k = k + 2;
            }
        }
    };
    @Override
    public Intent registerReceiver(BroadcastReceiver receiver, IntentFilter filter) {
        return super.registerReceiver(receiver, filter);
    }
}
