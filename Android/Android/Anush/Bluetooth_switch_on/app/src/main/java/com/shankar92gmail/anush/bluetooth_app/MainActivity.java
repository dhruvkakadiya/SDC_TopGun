package com.shankar92gmail.anush.bluetooth_app;

import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    Button bt_turn_on_button;
    BluetoothAdapter bt_adapter;
    int BLUETOOTH_REQ = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        bt_turn_on_button = (Button)findViewById(R.id.button);

        bt_turn_on_button.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {

                bt_adapter = BluetoothAdapter.getDefaultAdapter();

                if(bt_adapter == null)
                {
                    Toast.makeText(getBaseContext(),"No bluetooth support on device", Toast.LENGTH_LONG).show();
                }
                else
                {
                    if(!bt_adapter.isEnabled())
                    {
                        Intent i = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                        startActivityForResult(i, BLUETOOTH_REQ);
                    }
                }

            }
        });

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public void onActivityResult(int req_code, int res_code, Intent data)
    {
        if(req_code == BLUETOOTH_REQ)
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
