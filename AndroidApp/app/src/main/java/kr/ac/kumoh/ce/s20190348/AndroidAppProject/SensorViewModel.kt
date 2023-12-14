package kr.ac.kumoh.ce.s20190348.AndroidAppProject

import android.util.Log
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.launch
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory

class SensorViewModel() : ViewModel() {
    private val SERVER_URL = "https://port-0-embedded-system-1gksli2alppq2t18.sel4.cloudtype.app/"
    private val sensorApi: SensorApi
    private val nfcWarning: NfcWarning
    private val _sensorList = MutableLiveData<List<Sensor>>()
    val sensorList: LiveData<List<Sensor>>
        get() = _sensorList

    init {
        val retrofit = Retrofit.Builder()
            .baseUrl(SERVER_URL)
            .addConverterFactory(GsonConverterFactory.create())
            .build()

        sensorApi = retrofit.create(SensorApi::class.java)
        nfcWarning = retrofit.create(NfcWarning::class.java)
        fetchData()
    }
    fun fetchData() {
        viewModelScope.launch {
            try {
                val response = sensorApi.getSensors()
                _sensorList.value = response
            } catch (e: Exception) {
                Log.e("fetchData()", e.toString())
            }
        }
    }

    public fun warning() {
        viewModelScope.launch {
            try {
                val warningResponse = nfcWarning.getWarning()
                Log.d("warning()", "Warning response: $warningResponse")
            } catch (e: Exception) {
                Log.e("warning()", e.toString())
            }
        }
    }
}