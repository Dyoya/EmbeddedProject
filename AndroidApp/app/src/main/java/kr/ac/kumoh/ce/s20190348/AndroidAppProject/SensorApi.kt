package kr.ac.kumoh.ce.s20190348.AndroidAppProject

import retrofit2.http.GET

interface SensorApi {
    @GET("sensor")
    suspend fun getSensors(): List<Sensor>
}