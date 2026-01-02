/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package LynxEngine.Android;

import android.app.Activity;
import android.os.Bundle;
import android.content.Intent;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.LinearLayout;
import android.widget.EditText;
import LynxEngine.Android.JNI;


public class TextViewActivity extends Activity
{
	EditText mEditText;

    public TextViewActivity()
    {
	    super();
		
		JNI.LOGI("TextViewActivity()");
    }
	
	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);

		// create layout
		LinearLayout layout = new LinearLayout(this);
		layout.setOrientation(LinearLayout.VERTICAL);
		LayoutParams layoutParam = new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT); 
			
		// create webView
		mEditText = new EditText(this);
		mEditText.setLayoutParams(layoutParam);
		layout.addView(mEditText);
			
		setContentView(layout, layoutParam);
	}
}
