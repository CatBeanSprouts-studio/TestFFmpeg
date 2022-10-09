package com.mk.testffmpeg

import android.os.Bundle
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.tooling.preview.Preview

class MainActivity : ComponentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val filename = "${externalCacheDir?.absolutePath}/small_bunny_1080p_60fps.mp4"
        setContent {
            MessageCard(stringFromJNI(filename, externalCacheDir?.absolutePath ?: ""))
        }
    }

    @Preview
    @Composable
    fun MessageCardProxy() {
        MessageCard("啦啦啦了")
    }


    @Composable
    fun MessageCard(content: String) {
        Text(text = content)
    }

    /**
     * A native method that is implemented by the 'testffmpeg' native library,
     * which is packaged with this application.
     */
    private external fun stringFromJNI(filename: String, cacheDir: String): String

    companion object {
        // Used to load the 'testffmpeg' library on application startup.
        init {
            System.loadLibrary("testffmpeg")
        }
    }
}