package com.example.myapplication

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.widget.Toast
import androidx.activity.result.ActivityResultCallback
import androidx.activity.result.ActivityResultLauncher
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.example.myapplication.databinding.ActivityMainBinding
import java.io.IOException
import java.util.*


class MainActivity : AppCompatActivity() {

    //bluetooth device which will be used for connecting
    lateinit var myBltDevice: BluetoothDevice


    // Binding for buttons
    lateinit var binding: ActivityMainBinding
    // Bluetooth and permission variables
    lateinit var bluetoothManager: BluetoothManager
    lateinit var bluetoothAdapter: BluetoothAdapter
    lateinit var takePermission: ActivityResultLauncher<String>
    lateinit var takeResultLauncher: ActivityResultLauncher<Intent>

    //UUID witch is specific for HC-05/HC-06 bluetooth modules
    val MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")

    //Bluetooth Socket for communication with HC module
    private lateinit var btSocket: BluetoothSocket;

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)


        bluetoothManager = getSystemService(BLUETOOTH_SERVICE) as BluetoothManager
        bluetoothAdapter = bluetoothManager.adapter

        // Permission
        takePermission = registerForActivityResult(ActivityResultContracts.RequestPermission()) {
            if (it) {
                val intent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
                takeResultLauncher.launch(intent)
            } else {
                Toast.makeText(
                    applicationContext,
                    "Bluetooth Permission is not Granted",
                    Toast.LENGTH_SHORT
                ).show()
            }
        }
        takeResultLauncher =
            registerForActivityResult(ActivityResultContracts.StartActivityForResult(),
                ActivityResultCallback { result ->
                    if (result.resultCode == RESULT_OK) {
                        Toast.makeText(applicationContext, "Bluetooth enabled", Toast.LENGTH_SHORT)
                            .show()
                    } else {
                        Toast.makeText(applicationContext, "Bluetooth disabled", Toast.LENGTH_SHORT)
                            .show()
                    }
                })
        // button for Bluetooth enable
        binding.btnBluetoothOn.setOnClickListener() {
            takePermission.launch(android.Manifest.permission.BLUETOOTH_CONNECT)
        }
        //button for Bluetooth disable
        binding.btnBluetoothOff.setOnClickListener() {
            if (ActivityCompat.checkSelfPermission(
                    applicationContext,
                    Manifest.permission.BLUETOOTH_CONNECT
                ) == PackageManager.PERMISSION_GRANTED
            ) {
                bluetoothAdapter.disable()
                Toast.makeText(applicationContext, "Bluetooth Disabled", Toast.LENGTH_SHORT).show()
            }

        }

        // Getting paired devices
        binding.btnBluetoothPaired.setOnClickListener() {
            val data: StringBuffer = StringBuffer() //Buffer for paired Devices
            val pairedDevices = bluetoothAdapter.bondedDevices // Get paired devices
            // pushing devices data in buffer and searching for HC module
            for (device in pairedDevices) {
                data.append("Device name: " + device.name + "\nDevice address: " + device.address)
                if (device.name == "HC-05" || device.name == "HC-06") {
                    myBltDevice = device
                }
            }
            if (data.isEmpty()) {
                Toast.makeText(
                    applicationContext,
                    "Paired devices are not found",
                    Toast.LENGTH_LONG
                ).show()
            } else {
                if(myBltDevice.name == "HC-05" || myBltDevice.name =="HC-06"){
                    Toast.makeText(applicationContext,myBltDevice.name+"\n"+myBltDevice.address, Toast.LENGTH_SHORT).show()
                }
                else {
                    Toast.makeText(applicationContext, "You have paired devices, but HC is not paired", Toast.LENGTH_SHORT).show()
                }
            }
        }

        binding.btnConnect.setOnClickListener() {
            // Creating socket
            btSocket =myBltDevice.createRfcommSocketToServiceRecord(MY_UUID)
                try {
                    btSocket.connect()
                    Toast.makeText(this, "Connected to HC module", Toast.LENGTH_SHORT).show()
                } catch (e: Exception) {
                    Toast.makeText(this, "Can't Connect to HC module", Toast.LENGTH_SHORT).show()
                }
            // }
            //var mConnectThread = ConnectThread(myBltDevice,this)
            //mConnectThread.start();

        }
        binding.btnSend.setOnClickListener(){
            sendCommand("Test message")
        }
        binding.btnLED1.setOnClickListener(){
            sendCommand("PD12#1\n")
        }
        binding.btnLED2.setOnClickListener(){
            sendCommand("PD13#1\n")
        }
        binding.btnLED3.setOnClickListener(){
            sendCommand("PD14#1\n")
        }
        binding.btnLED4.setOnClickListener(){
            sendCommand("PD15#1\n")
        }

        }

    private fun sendCommand(input: String) {
        if (btSocket != null) {
            try{
                btSocket!!.outputStream.write(input.toByteArray())
                Toast.makeText(this, "Message sent:\n"+input, Toast.LENGTH_SHORT).show()
            } catch(e: IOException) {
                e.printStackTrace()
                Toast.makeText(this, "Can't send", Toast.LENGTH_SHORT).show()
            }
        }
    }

        //var mConnectThread = ConnectThread(this)
        //mConnectThread.start();

        //binding.btnLEDon.setOnClickListener(){
            //sendCommand("aaaaa")
       // }

//        binding.btnConnect.setOnClickListener() {
//            GlobalScope.launch {
//                var conBluetoothSocket: BluetoothSocket? = null
//                lateinit var conBluetoothDevice: BluetoothDevice
//                val MY_UIDD = UUID.fromString("6B0D7AE9-8E81-436E-9A9B-0DBB97929211")
//                var isConnected = false
//                try {
//                    if (conBluetoothSocket == null || !isConnected) {
//                        conBluetoothDevice = myBltDevice
//                        conBluetoothSocket = conBluetoothDevice.createRfcommSocketToServiceRecord(MY_UIDD)
//                        conBluetoothSocket!!.connect()
//                    }
//                } catch (e: IOException) {
//                    Toast.makeText(applicationContext, "Cant connect", Toast.LENGTH_SHORT).show()
//                }
//            }
//        }
//    }

//    private inner class ConnectThread(device: BluetoothDevice, ctx: Context): Thread(){
//        private lateinit var mmSocket: BluetoothSocket;
//        private lateinit var mmDevice: BluetoothDevice;
//        val MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");
//        public fun ConnectThread(device: BluetoothDevice, ctx: Context) {
//            var tmp: BluetoothSocket? = null;
//            mmDevice = device;
//            try {
//                if (ActivityCompat.checkSelfPermission(
//                        ctx,
//                        Manifest.permission.BLUETOOTH_CONNECT
//                    ) == PackageManager.PERMISSION_GRANTED
//                ) {
//                    tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
//                }
//
//            } catch (e: IOException ) { }
//            mmSocket = tmp!!;
//        }
//        fun run(ctx: Context) {
//            Toast.makeText(this@MainActivity, "Connected", Toast.LENGTH_SHORT).show()
//            if (ActivityCompat.checkSelfPermission(
//                    ctx,
//                    Manifest.permission.BLUETOOTH_SCAN
//                ) == PackageManager.PERMISSION_GRANTED
//            ) {
//                bluetoothAdapter.cancelDiscovery();
//            }
//            try {
//                Toast.makeText(this@MainActivity, "Connected", Toast.LENGTH_SHORT).show()
//                mmSocket.connect();
//            } catch (connectException: IOException) {
//                try {
//                    mmSocket.close();
//                } catch (closeException : IOException) { }
//                return
//            }
//        }
//        fun cancel() {
//            try {
//                mmSocket.close();
//            } catch (e: IOException ) { }
//        }
//
//    }


}
