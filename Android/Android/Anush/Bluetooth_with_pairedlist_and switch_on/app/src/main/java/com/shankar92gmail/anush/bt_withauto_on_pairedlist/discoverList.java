package com.shankar92gmail.anush.bt_withauto_on_pairedlist;

import android.app.Activity;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class discoverList extends Activity {
    ListView disclview;
    String[] discpaires;
    ArrayAdapter<String> disc_str_adap;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.discoveredlist_layout);
        disclview = (ListView)findViewById(R.id.listView);
        Bundle discbn = getIntent().getExtras();
        discpaires = discbn.getStringArray("discpaires");

        disc_str_adap = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1,discpaires);
        disclview.setAdapter(disc_str_adap);


    }
}

