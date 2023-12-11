package kr.ac.kumoh.ce.s20190348.AndroidAppProject

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.viewModels
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.lazy.itemsIndexed
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.livedata.observeAsState
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.layout.HorizontalAlignmentLine
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import coil.compose.AsyncImage
import kr.ac.kumoh.ce.s20190348.AndroidAppProject.ui.theme.AndroidAppTheme

class MainActivity : ComponentActivity() {
    private val viewModel: SensorViewModel by viewModels()
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            MainScreen(viewModel)
        }
    }
}

@Composable
fun MainScreen(viewModel: SensorViewModel) {
    val sensorList by viewModel.sensorList.observeAsState(emptyList())

    AndroidAppTheme {
        Surface(modifier = Modifier.fillMaxSize(), color = MaterialTheme.colorScheme.background) {
            SensorList(sensorList)
        }
    }
}

@Composable
fun SensorList(sensor: List<Sensor>)
{
    var colorList = dangerColor(sensor[0]) // 임시

    Column (
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
    ){
        Row(
            modifier = Modifier
                .fillMaxHeight()
                .weight(1f)
                .background(colorList[0])
                .padding(8.dp),
            verticalAlignment = Alignment.CenterVertically
        ){
            AsyncImage(
                model = "https://picsum.photos/300/300?random=$1",
                contentDescription = "온도 이미지",
                contentScale = ContentScale.Crop,
                modifier = Modifier
                    .size(100.dp)
                    .clip(RoundedCornerShape(percent = 10)),
            )
            Spacer(modifier = Modifier.width(10.dp))
            Column(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(16.dp),
                verticalArrangement = Arrangement.Center
            ){
                TitleText("온도 센서")
                DataText("10도")
            }
        }

        Spacer(modifier = Modifier.height(10.dp))

        Row(
            modifier = Modifier
                .fillMaxHeight()
                .weight(1f)
                .background(colorList[1])
                .padding(8.dp),
            verticalAlignment = Alignment.CenterVertically
        ){
            AsyncImage(
                model = "https://picsum.photos/300/300?random=$2",
                contentDescription = "수위 이미지",
                contentScale = ContentScale.Crop,
                modifier = Modifier
                    .size(100.dp)
                    .clip(RoundedCornerShape(percent = 10)),
            )
            Spacer(modifier = Modifier.width(10.dp))
            Column(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(16.dp),
                verticalArrangement = Arrangement.Center
            ){
                TitleText("수위 센서")
                DataText("10mm")
            }
        }

        Spacer(modifier = Modifier.height(10.dp))

        Row(
            modifier = Modifier
                .fillMaxHeight()
                .weight(1f)
                .background(colorList[2])
                .padding(8.dp),
            verticalAlignment = Alignment.CenterVertically
        ){
            AsyncImage(
                model = "https://picsum.photos/300/300?random=$3",
                contentDescription = "가스 이미지",
                contentScale = ContentScale.Crop,
                modifier = Modifier
                    .size(100.dp)
                    .clip(RoundedCornerShape(percent = 10)),
            )
            Spacer(modifier = Modifier.width(10.dp))
            Column(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(16.dp),
                verticalArrangement = Arrangement.Center
            ){
                TitleText("가스 센서")
                DataText("10")
            }
        }

        Spacer(modifier = Modifier.height(10.dp))

        Row(
            modifier = Modifier
                .fillMaxHeight()
                .weight(1f)
                .background(colorList[3])
                .padding(8.dp),
            verticalAlignment = Alignment.CenterVertically
        ){
            AsyncImage(
                model = "https://picsum.photos/300/300?random=$4",
                contentDescription = "NFC 이미지",
                contentScale = ContentScale.Crop,
                modifier = Modifier
                    .size(100.dp)
                    .clip(RoundedCornerShape(percent = 10)),
            )
            Spacer(modifier = Modifier.width(10.dp))

            Column(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(16.dp),
                verticalArrangement = Arrangement.Center
            ){
                TitleText("NFC")
                DataText("---")
            }
        }
    }
}

@Composable
fun TitleText(title: String)
{
    Text(title, fontSize = 30.sp)
}

@Composable
fun DataText(data: String) {
    Text(data, fontSize = 16.sp)
}

@Composable
fun dangerColor(sensor: Sensor): MutableList<Color> {
    var danger = Color(255, 0, 0, 100)
    var red = Color(255, 0, 0, 50)
    var yellow = Color(255, 255, 0, 50)
    var green = Color(0, 255, 0, 50)

    val colorList = mutableListOf<Color>()

    // 온도 센서
    if(sensor.temperature < -5 || sensor.temperature > 70)
        colorList[0] = danger
    else if(sensor.temperature < 0 || sensor.temperature > 60)
        colorList[0] = red
    else if (sensor.temperature < 5 || sensor.temperature > 40)
        colorList[0] = yellow
    else
        colorList[0] = green

    // 수위 센서
    if(sensor.water > 700)
        colorList[1] = danger
    else if(sensor.water > 500)
        colorList[1] = red
    else if (sensor.water > 300)
        colorList[1] = yellow
    else
        colorList[1] = green

    // 가스 센서
    if(sensor.temperature > 50)
        colorList[2] = danger
    else if(sensor.temperature > 30)
        colorList[2] = red
    else if (sensor.temperature > 10)
        colorList[2] = yellow
    else
        colorList[2] = green

    // NFC


    return colorList
}