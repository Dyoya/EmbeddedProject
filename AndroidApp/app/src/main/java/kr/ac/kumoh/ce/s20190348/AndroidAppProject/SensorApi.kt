package kr.ac.kumoh.ce.s20190348.AndroidAppProject

import retrofit2.http.GET

interface SensorApi {
    @GET("sensor")
    suspend fun getSensors(): List<Sensor>
}

interface NfcWarning {
    @GET("warning")
    suspend fun getWarning(): List<Warning>
}
