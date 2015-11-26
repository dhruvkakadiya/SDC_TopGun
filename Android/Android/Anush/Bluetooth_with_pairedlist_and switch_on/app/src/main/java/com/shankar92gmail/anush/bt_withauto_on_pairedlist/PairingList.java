package com.shankar92gmail.anush.bt_withauto_on_pairedlist;

import android.app.Activity;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class PairingList extends Activity {
    ListView lview;
    String[] paires;
    ArrayAdapter<String> str_adap;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.pairinglist_layout);
        lview = (ListView)findViewById(R.id.listView);
        Bundle bn = getIntent().getExtras();
        paires = bn.getStringArray("paires");

        str_adap = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1,paires);
        lview.setAdapter(str_adap);


    }
}

