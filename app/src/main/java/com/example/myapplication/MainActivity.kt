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

    // bluetooth device koji cemo kasnije koristiti za uspostavljanje konekcije
    lateinit var myBltDevice: BluetoothDevice


    // Binding za buttone
    lateinit var binding: ActivityMainBinding
    // Bluetooth i permission varijable
    lateinit var bluetoothManager: BluetoothManager
    lateinit var bluetoothAdapter: BluetoothAdapter
    lateinit var takePermission: ActivityResultLauncher<String>
    lateinit var takeResultLauncher: ActivityResultLauncher<Intent>

    val MY_UIDD = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")

    private lateinit var btSocket: BluetoothSocket;

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)


        bluetoothManager = getSystemService(BLUETOOTH_SERVICE) as BluetoothManager
        bluetoothAdapter = bluetoothManager.adapter
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
                        Toast.makeText(applicationContext, "Bluetooth ON", Toast.LENGTH_SHORT)
                            .show()
                    } else {
                        Toast.makeText(applicationContext, "Bluetooth OFF", Toast.LENGTH_SHORT)
                            .show()
                    }
                })
        binding.btnBluetoothOn.setOnClickListener() {
            takePermission.launch(android.Manifest.permission.BLUETOOTH_CONNECT)
        }
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
        binding.btnBluetoothPaired.setOnClickListener() {
            val data: StringBuffer = StringBuffer()
            //val data = arrayListOf<String>()
            val pairedDevices = bluetoothAdapter.bondedDevices
            for (device in pairedDevices) {
                data.append("Device name: " + device.name + "\nDevice address: " + device.address)
                if (device.name == "HC-05" || device.name == "HC-06") {
                    myBltDevice = device
                    //myBltDevice.name=device.name
                    //myBltDevice.address=device.address
                }
            }
            if (data.isEmpty()) {
                Toast.makeText(
                    applicationContext,
                    "Paired devices are not found",
                    Toast.LENGTH_LONG
                ).show()
            } else {
                Toast.makeText(applicationContext, data, Toast.LENGTH_SHORT).show()
            }
        }

        binding.btnBluetoothHC.setOnClickListener() {
            btSocket =myBltDevice.createRfcommSocketToServiceRecord(MY_UIDD)
            //btSocket=myBltDevice.createInsecureRfcommSocketToServiceRecord(MY_UIDD)
                try {
                    btSocket.connect()
                    Toast.makeText(this, "Connected", Toast.LENGTH_SHORT).show()
                } catch (e: Exception) {
                    Toast.makeText(this, "Can't Connect", Toast.LENGTH_SHORT).show()
                }
            // }
            //var mConnectThread = ConnectThread(myBltDevice,this)
            //mConnectThread.start();

        }
        binding.btnSend.setOnClickListener(){
            sendCommand("Indir salje poruku\n")
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
