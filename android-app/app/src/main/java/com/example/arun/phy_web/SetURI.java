package com.example.arun.phy_web;

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class SetURI extends Activity {
    private final static String TAG = SetURI.class.getSimpleName();

    public static final String EXTRAS_DEVICE = "EXTRAS_DEVICE";
    private TextView tv = null;
    private EditText et = null;
    private Button btn = null;
    private String mDeviceName;
    private String mDeviceAddress;
    private BLEService mBluetoothLeService;
    private Map<UUID, BluetoothGattCharacteristic> map = new HashMap<UUID, BluetoothGattCharacteristic>();

    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName,
                                       IBinder service) {
            mBluetoothLeService = ((BLEService.LocalBinder) service)
                    .getService();
            if (!mBluetoothLeService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                finish();
            }
            // Automatically connects to the device upon successful start-up
            // initialization.
            mBluetoothLeService.connect(mDeviceAddress);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mBluetoothLeService = null;
        }
    };

    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();

            if (BLEService.ACTION_GATT_DISCONNECTED.equals(action)) {
            } else if (BLEService.ACTION_GATT_SERVICES_DISCOVERED
                    .equals(action)) {
                getGattService(mBluetoothLeService.getSupportedGattService());
            } else if (BLEService.ACTION_DATA_AVAILABLE.equals(action)) {
                displayData(intent.getByteArrayExtra(BLEService.EXTRA_DATA));
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.uri_send);

        tv = (TextView) findViewById(R.id.textView);
        //tv.setMovementMethod(ScrollingMovementMethod.getInstance());
        et = (EditText) findViewById(R.id.editText);
        btn = (Button) findViewById(R.id.send);
        btn.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                final BluetoothGattCharacteristic characteristic = map
                        .get(BLEService.UUID_BLE_SHIELD_TX);

                final String str = et.getText().toString();
                byte b = 0x00;

                byte[] tmp = str.getBytes();
                byte[] tx = new byte[tmp.length + 1];
                tx[0] = b;
                for (int i = 1; i < tmp.length + 1; i++) {
                    tx[i] = tmp[i - 1];
                }

                characteristic.setValue(tx);
                mBluetoothLeService.writeCharacteristic(characteristic);

                et.setText("");
                tv.setText(str);
                /*Call URL Shortener here.
                The converted URL will be of the form http://goo.gl/ABCDEF.
                Send only ABCDEF and then on receiver side reform it into
                http://goo.gl/ABCDEF and broadcast.
                */
                // Create the callback object to set the url
                /*
                PwsClient.ShortenUrlCallback urlSetter = new PwsClient.ShortenUrlCallback() {
                    @Override
                    public void onUrlShortened(String newUrl) {
                        byte b = 0x00;
                        //Now fetch only the last 6 characters
                        String urlSend = newUrl.substring(14);

                        byte[] tmp = urlSend.getBytes();
                        byte[] tx = new byte[tmp.length + 1];
                        tx[0] = b;
                        for (int i = 1; i < tmp.length + 1; i++) {
                            tx[i] = tmp[i - 1];
                        }

                        characteristic.setValue(tx);
                        mBluetoothLeService.writeCharacteristic(characteristic);

                        et.setText("");
                        tv.setText(newUrl);
                    }
                    @Override
                    public void onError(String oldUrl) {
                        // Avoid showing a "URL too long" error if we shortened due to non-ASCII chars in url
                        int errorMessageStringId = R.string.url_shortening_error;
                        Toast.makeText(SetURI.this, errorMessageStringId, Toast.LENGTH_SHORT).show();
                    }
                };

                    // Shorten the url if necessary
                    //PwsClient.getInstance(getActivity()).shortenUrl(str, urlSetter, TAG);
                PwsClient.getInstance(SetURI.this).shortenUrl(str, urlSetter, TAG);
                */

            }
        });

        Intent intent = getIntent();

        mDeviceAddress = intent.getStringExtra(Device.EXTRA_DEVICE_ADDRESS);
        mDeviceName = intent.getStringExtra(Device.EXTRA_DEVICE_NAME);

        getActionBar().setTitle(mDeviceName);
        getActionBar().setDisplayHomeAsUpEnabled(true);

        Intent gattServiceIntent = new Intent(this, BLEService.class);
        bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE);
    }

    @Override
    protected void onResume() {
        super.onResume();

        registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            mBluetoothLeService.disconnect();
            mBluetoothLeService.close();

            System.exit(0);
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onStop() {
        super.onStop();

        unregisterReceiver(mGattUpdateReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        mBluetoothLeService.disconnect();
        mBluetoothLeService.close();

        System.exit(0);
    }

    private void displayData(byte[] byteArray) {
        if (byteArray != null) {
            String data = new String(byteArray);
            tv.append(data);
            // find the amount we need to scroll. This works by
            // asking the TextView's internal layout for the position
            // of the final line and then subtracting the TextView's height
            final int scrollAmount = tv.getLayout().getLineTop(
                    tv.getLineCount())
                    - tv.getHeight();
            // if there is no need to scroll, scrollAmount will be <=0
            if (scrollAmount > 0)
                tv.scrollTo(0, scrollAmount);
            else
                tv.scrollTo(0, 0);
        }
    }

    private void getGattService(BluetoothGattService gattService) {
        if (gattService == null)
            return;

        BluetoothGattCharacteristic characteristic = gattService
                .getCharacteristic(BLEService.UUID_BLE_SHIELD_TX);
        map.put(characteristic.getUuid(), characteristic);

        BluetoothGattCharacteristic characteristicRx = gattService
                .getCharacteristic(BLEService.UUID_BLE_SHIELD_RX);
        mBluetoothLeService.setCharacteristicNotification(characteristicRx,
                true);
        mBluetoothLeService.readCharacteristic(characteristicRx);
    }

    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();

        intentFilter.addAction(BLEService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BLEService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BLEService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(BLEService.ACTION_DATA_AVAILABLE);

        return intentFilter;
    }
}
