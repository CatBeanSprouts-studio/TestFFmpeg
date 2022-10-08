package com.mk.testffmpeg

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.mk.testffmpeg.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val filename = "${externalCacheDir?.absolutePath}/small_bunny_1080p_60fps.mp4"

        // Example of a call to a native method
        binding.sampleText.text = stringFromJNI(filename, externalCacheDir?.absolutePath ?: "")
    }

    /**
     * A native method that is implemented by the 'testffmpeg' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(filename: String, cacheDir: String): String

    companion object {
        // Used to load the 'testffmpeg' library on application startup.
        init {
            System.loadLibrary("testffmpeg")
        }
    }
}