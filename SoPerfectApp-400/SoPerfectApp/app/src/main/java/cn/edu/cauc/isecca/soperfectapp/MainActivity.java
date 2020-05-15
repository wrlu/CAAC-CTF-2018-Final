package cn.edu.cauc.isecca.soperfectapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private EditText flagText;

    static {
        System.loadLibrary("native-lib");
        Log.e("MainActivity","Load Library");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        flagText = findViewById(R.id.editTextFlag);
        Button btnSubmit = findViewById(R.id.btnSubmit);
        btnSubmit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String flag = flagText.getText().toString();
                check(flag);
            }
        });
    }

    public native String stringFromJNI();

    public void check(String flag) {
        boolean result;
        result = realcheck(flag);
        Toast.makeText(this, result ? "RIGHT" : "WRONG", Toast.LENGTH_LONG).show();
    }

    public boolean realcheck(String flag) {
        return false;
    }

    public static native boolean check1(String flag);
    public static native boolean check2(String flag);


}
