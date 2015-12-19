package com.strangelyhuman.rccarnavigator;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.net.ConnectivityManager;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import android.support.v7.widget.Toolbar;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.UiSettings;
import com.google.android.gms.maps.model.BitmapDescriptor;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.gms.maps.model.PolylineOptions;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Method;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.UUID;
import java.util.Vector;

import javax.xml.transform.Source;


    public class MapsActivity extends FragmentActivity {

        private GoogleMap mMap; // Might be null if Google Play services APK is not available.
        ArrayList<LatLng> MarkerPoints;


        private static String TAG = "Maps Activity";
        String CarRoute = "";
        int Count_Ordinates = 0;
        boolean startButtonVariable = false;
        double destinationLat;
        double destinationLon;
        int lat_lon_count = 1;

        // source location lat and long for setting source marker automatically
        double source_lat;
        double source_long;
        LatLng CarSource;

        Button btnOn, btnOff,btOn,btConnect,btdisconnect, sndrt, carLocation;
        int BT_CONNECT_CODE = 1;
        int connect = 0;
        int start_stop = 0;
        int snd_route_en = 0;
        int carUpdate = 0;
        Handler mHandler;
        TextView txt;

        final int RECEIVE_MESSAGE = 1;        // Status  for Handler

        private BluetoothAdapter btAdapter;
        private BluetoothSocket btSocket;

        private ConnectedThread mConnectedThread;

        // SPP UUID service
        private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

        //HC-06 module MAC -- slave only
        private static String address = "20:15:03:03:09:75";

        //HC=05 module MAC --  master/slave
        //private static String address = "20:15:08:13:10:18";

        private static String tx_data1 = "0 \n";
       // private static String tx_data2 = "0 \n";

        String disp_temp_str;

        public boolean isConnected() {
            ConnectivityManager connectivityManager = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);

            return connectivityManager.getActiveNetworkInfo() != null && connectivityManager.getActiveNetworkInfo().isConnectedOrConnecting();
        }

        @Override
        protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_maps);
            //Toolbar myToolbar = (Toolbar) findViewById(R.id.my_toolbar);
            setUpMapIfNeeded();
            mMap.setMyLocationEnabled(true);

            btnOn = (Button) findViewById(R.id.btnOn);
            btnOff = (Button) findViewById(R.id.btnOff);
            btOn = (Button) findViewById(R.id.btOn);
            btConnect = (Button) findViewById(R.id.btConnect);
            btdisconnect = (Button)findViewById(R.id.btdisconnect);
            sndrt = (Button) findViewById(R.id.sndrt);
            txt = (TextView) findViewById(R.id.txt);
            carLocation = (Button) findViewById(R.id.carLoc);
            btAdapter = BluetoothAdapter.getDefaultAdapter();

            //Car Source Marker Options

            carLocation.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (start_stop == 1 && carUpdate == 1) {
                        LatLng lCarSource = new LatLng(source_lat, source_long);

                        CarSource = lCarSource;
                        Toast.makeText(getApplicationContext(), CarSource.latitude + " " + CarSource.longitude,Toast.LENGTH_LONG ).show();
                        mMap.addMarker(new MarkerOptions().position(CarSource).icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED)));
                    }
                }
            });

            btOn.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    checkBTState();
                }
            });

            btnOn.setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                   // mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(CarSource, 6));
                    if(connect == 1 && start_stop == 0) {
                        mConnectedThread.sendData(tx_data1);
                        start_stop = 1;
                        Toast.makeText(getBaseContext(), "Car Start", Toast.LENGTH_SHORT).show();
                    }
                }
            });

            btnOff.setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                    if(connect == 1 && start_stop == 1) {
                        mConnectedThread.sendData(tx_data1);
                        start_stop = 0;
                        Toast.makeText(getBaseContext(), "Car Stop", Toast.LENGTH_SHORT).show();
                    }
                }
            });

            btConnect.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (connect == 0) {
                        connect();
                    }
                }
            });

            btdisconnect.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if(connect == 1) {
                        disconnect();
                    }
                }
            });

            sndrt.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if((connect == 1) && (snd_route_en == 1)) {
                        mConnectedThread.sendData("" + Count_Ordinates + " " + CarRoute + "\n");
                        Toast.makeText(getBaseContext(), "Sending Route...", Toast.LENGTH_SHORT).show();
                        Log.d(TAG, CarRoute);
                    }
                }
            });

            mHandler = new Handler() {
                @Override
                public void handleMessage(Message msg) {
                    byte[] writeBuf = (byte[]) msg.obj;
                    int begin = (int)msg.arg1;
                    int end = (int)msg.arg2;

                    switch(msg.what) {
                        case 1:
                            String writeMessage = new String(writeBuf);
                            writeMessage = writeMessage.substring(begin, end);
                            disp_temp_str = writeMessage;
                            if(lat_lon_count == 1)
                            {
                                source_lat = Double.parseDouble(writeMessage);
                                Log.d(TAG, Double.toString(source_lat));
                                Log.d(TAG, disp_temp_str);
                                lat_lon_count = 2;
                            }
                            else if(lat_lon_count == 2)
                            {
                                source_long = Double.parseDouble(writeMessage);
                                Log.d(TAG, Double.toString(source_long));
                                Log.d(TAG, disp_temp_str);
                                lat_lon_count = 1;
                                carUpdate = 1;
                            }

                            txt.setText("Data from HC-05: " + source_lat + " " + source_long);            // update TextView
                            break;
                    }
                }
            };


            //check if your phone is connected to the internet
            if An(!isConnected()) {
                Toast.makeText(MapsActivity.this, "Please ensure that you are connected to the internet", Toast.LENGTH_LONG).show();
            }

            //location marker options
            final MarkerOptions locationMarkerOptions = new MarkerOptions();
            locationMarkerOptions.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED));
            locationMarkerOptions.title("Source");
            locationMarkerOptions.draggable(false);

            //location button override
            mMap.setOnMyLocationButtonClickListener(new GoogleMap.OnMyLocationButtonClickListener() {
                @Override
                public boolean onMyLocationButtonClick() {
                    //check if start_stop variable is equal to 1 and place a marker at SourceLat and SourceLong
                    Toast.makeText(MapsActivity.this, "Finding your location...", Toast.LENGTH_SHORT).show();
                    return false;
                }
            });

            //initialize your latlng array
            MarkerPoints = new ArrayList<LatLng>();
            mMap.setOnMapClickListener(new GoogleMap.OnMapClickListener() {
                @Override
                public void onMapClick(LatLng point) {

                    //check if the no of markers on map is > 2
                    if (MarkerPoints.size() >= 2) {
                        MarkerPoints.clear();
                        mMap.clear();
                        return;
                    }


                    //Adding a new point
                    MarkerPoints.add(point);

                    //create new marker options
                    MarkerOptions options = new MarkerOptions();

                    //set the position of the marker
                    options.position(point);

                    if (MarkerPoints.size() == 1) {
                        options.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_GREEN));
                        options.title("Destination" + " " + point.latitude + " " + point.longitude);
                        options.draggable(true);
                        Toast.makeText(MapsActivity.this, "Add a Destination Marker", Toast.LENGTH_SHORT).show();
                    }

//                    if (MarkerPoints.size() == 1) {
//                        options.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED));
//                        options.draggable(true);
//                        options.title("Source");
//                        Toast.makeText(MapsActivity.this, "Add a Origin Marker", Toast.LENGTH_SHORT).show();
//                    }


//                    if (MarkerPoints.size() == 2) {
//                        options.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_GREEN));
//                        options.title("Destination" + " " + point.latitude + " " + point.longitude);
//                        options.draggable(true);
//                        Toast.makeText(MapsActivity.this, "Add a Destination Marker", Toast.LENGTH_SHORT).show();
//                    }

                    // DestinationMarker += MarkerPoints.get(1).latitude + " " + MarkerPoints.get(1).longitude;





                    //add marker to the map
                    mMap.addMarker(options);
                    if (MarkerPoints.size() == 1) {
                        //LatLng origin = MarkerPoints.get(0); //set your origin point
                        LatLng destination = MarkerPoints.get(0);
                        //getting a string for the directions api
                        int tempLat = (int) (destination.latitude * 1000000);
                        destinationLat = tempLat / 1000000d;
                        int tempLon = (int) (destination.longitude * 1000000);
                        destinationLon = tempLon / 1000000d;
                        Log.d(TAG, Double.toString(destinationLat) + " " + Double.toString(destinationLon));
                        String url = getDirectionsUrl(CarSource, destination);
                        DownloadTask downloadTask1 = new DownloadTask();
                        downloadTask1.execute(url);
                        snd_route_en = 1;
                    }
                }
            });
        }

        private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {
            if(Build.VERSION.SDK_INT >= 10){
                try {
                    final Method m = device.getClass().getMethod("createInsecureRfcommSocketToServiceRecord", new Class[] { UUID.class });
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


        private void disconnect() {
            try {
                btSocket.close();
                connect = 0;
                Toast.makeText(getApplicationContext(), "Disconnected", Toast.LENGTH_SHORT).show();
            } catch (IOException e) { }

        }

        private void connect() {
            // Set up a pointer to the remote node using it's address.
            BluetoothDevice device = btAdapter.getRemoteDevice(address);

            // Two things are needed to make a connection:
            //   A MAC address, which we got above.
            //   A Service ID or UUID.  In this case we are using the
            //     UUID for SPP.
            try {
                btSocket = createBluetoothSocket(device);
            } catch (IOException e1) {
                errorExit("Fatal Error", "In onResume() and socket create failed: " + e1.getMessage() + ".");
            }

            // Discovery is resource intensive.  Make sure it isn't going on
            // when you attempt to connect and pass your message.
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

            // Create a data stream so we can talk to server.
            Log.d(TAG, "...Create Socket...");

            mConnectedThread = new ConnectedThread(btSocket);
            mConnectedThread.start();
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

        private class ConnectedThread extends Thread {
            private final InputStream mmInStream;
            private final OutputStream mmOutStream;

            public ConnectedThread(BluetoothSocket socket) {
                InputStream tmpIn = null;
                OutputStream tmpOut = null;

                // Get the input and output streams, using temp objects because
                // member streams are final
                try {
                    tmpIn = socket.getInputStream();
                    tmpOut = socket.getOutputStream();
                } catch (IOException e) { }

                mmInStream = tmpIn;
                mmOutStream = tmpOut;
            }

            public void run() {
                byte[] buffer = new byte[1024];
                int begin = 0;
                int bytes = 0;
                while (true) {
                    try {
                        bytes += mmInStream.read(buffer, bytes, buffer.length - bytes);
                        for(int i = begin; i < bytes; i++) {
                            if(buffer[i] == "\n\r".getBytes()[0]) {
                                mHandler.obtainMessage(1, begin, i, buffer).sendToTarget();
                                begin = i + 1;
                                if(i == bytes - 1) {
                                    bytes = 0;
                                    begin = 0;
                                }
                            }
                        }
                    } catch (IOException e) {
                        break;
                    }
                }
            }

            private void sendData(String message) {
                byte[] msgBuffer = message.getBytes();

                Log.d(TAG, "...Send data: " + message + "...");

                try {
                    mmOutStream.write(msgBuffer);
                    Toast.makeText(getApplicationContext(),"data sent",Toast.LENGTH_LONG).show();
                } catch (IOException e) {
                    String msg = "In onResume() and an exception occurred during write: " + e.getMessage();
                    if (address.equals("00:00:00:00:00:00"))
                        msg = msg + ".\n\nUpdate your server address from 00:00:00:00:00:00 to the correct address on line 35 in the java code";
                    msg = msg +  ".\n\nCheck that the SPP UUID: " + MY_UUID.toString() + " exists on server.\n\n";

                    errorExit("Fatal Error", msg);
                }
            }

        }


        private String getDirectionsUrl(LatLng origin, LatLng destination) {
            //Origin of the route
            String originStr = "origin=" + origin.latitude + "," + origin.longitude;
            String destinationStr = "destination=" + destination.latitude + "," + destination.longitude;
            String output = "json";
            String parameters = originStr + "&" + destinationStr + "&mode=bicycling&key=AIzaSyCwfUYxgA3FXrxX6RqlOJVbf16lHGa7uSs";
            String path = origin.latitude + "," + origin.longitude + "|" + destination.latitude + "," + destination.longitude;
            //building the string for the web service
            String url = "https://maps.googleapis.com/maps/api/directions/" + output + "?" + parameters;
            //adding a road maps api
            //String url = "https://roads.googleapis.com/v1/snapToRoads?"+ path + "&key=AIzaSyCwfUYxgA3FXrxX6RqlOJVbf16lHGa7uSs";
            return url;
        }

        private String downloadUrl(String strUrl) throws IOException {
            String data = "";
            InputStream iStream = null;
            HttpURLConnection urlConnection = null;
            try {
                URL url = new URL(strUrl);

                //create a http connection
                urlConnection = (HttpURLConnection) url.openConnection();

                //Connecting to url
                urlConnection.connect();

                //reading data from Url
                iStream = urlConnection.getInputStream();

                BufferedReader br = new BufferedReader(new InputStreamReader(iStream));
                StringBuilder sb = new StringBuilder();

                String line = "";
                while ((line = br.readLine()) != null) {
                    sb.append(line);
                }

                data = sb.toString();
                br.close();
            } catch (Exception e) {
                Log.d("Exception download url", e.toString());
            } finally {
                iStream.close();
                urlConnection.disconnect();
            }
            Log.d(TAG, data);
            return data;
        }

        //fetches data from url passed - DownloadTask method
        private class DownloadTask extends AsyncTask<String, Void, String> {

            //download data in non-ui thread
            @Override
            protected String doInBackground(String... url) {

                //store data from the web service
                String data = "";
                try {
                    //fetching data from the web service
                    data = downloadUrl(url[0]);
                } catch (Exception e) {
                    Log.d("Background Task", e.toString());
                }
                return data;
            }

            //executes in UI thread after doInBackground
            @Override
            protected void onPostExecute(String result) {
                super.onPostExecute(result);

                //invokes thread for parsing json data
                ParserTask parserTask = new ParserTask();
                parserTask.execute(result);
            }

        }

        //defining ParserTask to parse JSON Data
        private class ParserTask extends AsyncTask<String, Integer, List<List<HashMap<String, String>>>> {

            //parsing the data in non-ui thread
            @Override
            protected List<List<HashMap<String, String>>> doInBackground(String... jsonData) {

                JSONObject jObject;
                List<List<HashMap<String, String>>> routes = null;

                try {
                    jObject = new JSONObject(jsonData[0]);
                    DirectionsJSONParser parser = new DirectionsJSONParser();

                    //start your parsing here
                    routes = parser.parse(jObject);
                } catch (Exception e) {
                    e.printStackTrace();
                }
                return routes;
            }

            //executes in UI thread
            @Override
            protected void onPostExecute(List<List<HashMap<String, String>>> result) {
                ArrayList<LatLng> points = null;
                PolylineOptions lineOptions = null;
                MarkerOptions markerOptions = new MarkerOptions();

                //traversing through all the routes
                //lineOptions = new PolylineOptions();
                for (int i = 0; i < result.size(); i++) {
                    points = new ArrayList<LatLng>();
                    lineOptions = new PolylineOptions();

                    //fetching ith route
                    List<HashMap<String, String>> path = result.get(i);

                    CarRoute = "";
                    Count_Ordinates = 0;
                    //fetching points in the ith route
                    for (int j = 1; j < path.size(); j++) {
                        HashMap<String, String> point = path.get(j);

                        double lat = Double.parseDouble(point.get("lat"));
                        double lng = Double.parseDouble(point.get("lng"));

                        LatLng position = new LatLng(lat, lng);
                        //CarRoute +=  "" + lat + " " + lng + " ";
                        //Log.d(TAG, CarRoute);
                        //Log.d(TAG, position.toString());
                        if ((j % 2 == 1)  && path.size() > 15) {
                            //skip this point
                            continue;
                        }
                            mMap.addMarker(markerOptions.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_BLUE))
                                    .position(position)
                                    .title("latitude: " + lat + ", " + "longitude: " + lng));

                            double templat_;
                            double templon_;

                            int temp_Lat = (int) (lat * 1000000);
                            templat_ = temp_Lat / 1000000d;
                            int temp_Lon = (int) (lng * 1000000);
                            templon_ = temp_Lon / 1000000d;


                            CarRoute += "" + templat_ + " " + templon_ + " ";
                            ++Count_Ordinates;


                        //plot points on the map
                        points.add(position);

                    }

                    //CarRoute += destinationLat + " " + destinationLon;
                    //Count_Ordinates +=1;
                    Log.d(TAG, CarRoute);
                    Log.d(TAG, Integer.toString(Count_Ordinates));

                    //check if the number of coordinates exceeds 15


                    //adding all points in the route to lineOptions
                    lineOptions.addAll(points);
                    lineOptions.width(5);
                    lineOptions.color(Color.BLUE);
                }
                //draw polyline on map
                mMap.addPolyline(lineOptions);
            }

        }


        @Override
        protected void onResume() {
            super.onResume();
            setUpMapIfNeeded();
        }

        /**
         * Sets up the map if it is possible to do so (i.e., the Google Play services APK is correctly
         * installed) and the map has not already been instantiated.. This will ensure that we only ever
         * call {@link #setUpMap()} once when {@link #mMap} is not null.
         * <p>
         * If it isn't installed {@link SupportMapFragment} (and
         * {@link com.google.android.gms.maps.MapView MapView}) will show a prompt for the user to
         * install/update the Google Play services APK on their device.
         * <p>
         * A user can return to this FragmentActivity after following the prompt and correctly
         * installing/updating/enabling the Google Play services. Since the FragmentActivity may not
         * have been completely destroyed during this process (it is likely that it would only be
         * stopped or paused), {@link #onCreate(Bundle)} may not be called again so we should call this
         * method in {@link #onResume()} to guarantee that it will be called.
         */
        private void setUpMapIfNeeded() {
            // Do a null check to confirm that we have not already instantiated the map.
            if (mMap == null) {
                // Try to obtain the map from the SupportMapFragment.
                mMap = ((SupportMapFragment) getSupportFragmentManager().findFragmentById(R.id.map))
                        .getMap();
                // Check if we were successful in obtaining the map.
                if (mMap != null) {
                    setUpMap();
                }
            }
        }

        /**
         * This is where we can add markers or lines, add listeners or move the camera. In this case, we
         * just add a marker near Africa.
         * <p>
         * This should only be called once and when we are sure that {@link #mMap} is not null.
         */
        private void setUpMap() {

            //app behavior when the app first starts up
            //centers the map at the SJSU campus
            LatLng SJSU = new LatLng(37.3351874, -121.8810715);
            mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(SJSU, 6));
            mMap.animateCamera(CameraUpdateFactory.newLatLngZoom(SJSU, 18));

            //Click Listeners

            Button button5 = (Button) findViewById(R.id.clearButton);
            button5.setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                    //Clear all markers on screen
                    snd_route_en = 0;
                    mMap.clear();
                }
            });

            //need to work on getting a location value

            //UI settings are added here
            UiSettings uiValue = mMap.getUiSettings();
            uiValue.setZoomControlsEnabled(true);
            uiValue.setMyLocationButtonEnabled(true); //only works if mMap.setLocationEnabled(true)
            uiValue.setCompassEnabled(true); //also doesn't seem to work

        }

    }




