package com.example.myapplication

import android.Manifest
import android.annotation.SuppressLint
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.content.Intent
import android.content.pm.PackageManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.ArrayAdapter
import android.widget.ListView
import android.widget.Toast
import androidx.activity.result.ActivityResultCallback
import androidx.activity.result.ActivityResultLauncher
import androidx.activity.result.contract.ActivityResultContract
import androidx.activity.result.contract.ActivityResultContracts
import androidx.core.app.ActivityCompat
import com.example.myapplication.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    //var myBltDevice = bltDevice("none","none")
    lateinit var myBltDevice: BluetoothDevice




    lateinit var binding: ActivityMainBinding
    lateinit var bluetoothManager: BluetoothManager
    lateinit var bluetoothAdapter: BluetoothAdapter
    lateinit var takePermission: ActivityResultLauncher<String>
    lateinit var takeResultLauncher: ActivityResultLauncher<Intent>

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding=ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)


        bluetoothManager=getSystemService(BLUETOOTH_SERVICE) as BluetoothManager
        bluetoothAdapter=bluetoothManager.adapter
        takePermission=registerForActivityResult(ActivityResultContracts.RequestPermission()){
            if(it){
                val intent=Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
                takeResultLauncher.launch(intent)
            }
            else{
                Toast.makeText(applicationContext,"Bluetooth Permission is not Granted",Toast.LENGTH_SHORT).show()
            }
        }
        takeResultLauncher=registerForActivityResult(ActivityResultContracts.StartActivityForResult(),
            ActivityResultCallback {
                result ->
                if(result.resultCode== RESULT_OK){
                    Toast.makeText(applicationContext,"Bluetooth ON",Toast.LENGTH_SHORT).show()
                }
                else{
                    Toast.makeText(applicationContext,"Bluetooth OFF",Toast.LENGTH_SHORT).show()
                }
            })
        binding.btnBluetoothOn.setOnClickListener(){
            takePermission.launch(android.Manifest.permission.BLUETOOTH_CONNECT)
        }
        binding.btnBluetoothOff.setOnClickListener(){
            if (ActivityCompat.checkSelfPermission(
                    applicationContext,
                    Manifest.permission.BLUETOOTH_CONNECT
                ) == PackageManager.PERMISSION_GRANTED
            ) {
                bluetoothAdapter.disable()
                Toast.makeText(applicationContext,"Bluetooth Disabled",Toast.LENGTH_SHORT).show()
            }

        }
        binding.btnBluetoothPaired.setOnClickListener(){
            val data:StringBuffer= StringBuffer()
            //val data = arrayListOf<String>()
            val pairedDevices = bluetoothAdapter.bondedDevices
            for( device in pairedDevices){
                data.append("Device name: "+device.name+"\nDevice address: "+device.address)
                if(device.name=="HC-05" || device.name=="HC-06"){
                    myBltDevice = device
                    //myBltDevice.name=device.name
                    //myBltDevice.address=device.address
                }
            }
            if(data.isEmpty()){
                Toast.makeText(applicationContext,"Paired devices are not found",Toast.LENGTH_LONG).show()
            } else{
               Toast.makeText(applicationContext,data,Toast.LENGTH_SHORT).show()
            }
        }

        binding.btnBluetoothHC.setOnClickListener(){
            Toast.makeText(applicationContext,myBltDevice.name+"\n"+myBltDevice.address,Toast.LENGTH_LONG).show()
        }

    }
}