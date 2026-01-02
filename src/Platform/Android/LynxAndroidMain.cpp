/*
 * Copyright (C) 2010 The Android Open Source Project
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
 *
 */

//#include <android/sensor.h>
//#include <android/log.h>
#include <Platform/Android/android_native_app_glue.h>
#include <LynxEngine_PCH.h>

#define MAX_SCREEN_HEIGHT					1280

using namespace LynxEngine;

LYNXBOOL gbInit;
/*
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
LynxEngine::GameFramework::CKernel* lynxCreateKernel()
{
	return new LynxEngine::GameFramework::Android::CKernel();
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void lynxReleaseKernel(LynxEngine::GameFramework::CKernel* lpk)
{
	delete lpk;
}
*/
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
static void engine_handle_cmd(struct android_app* app, int32_t cmd) 
{
    switch (cmd) 
	{
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            //engine->app->savedState = malloc(sizeof(struct saved_state));
            //*((struct saved_state*)engine->app->savedState) = engine->state;
            //engine->app->savedStateSize = sizeof(struct saved_state);
            break;

        case APP_CMD_INIT_WINDOW:		
			{
				if (!glpKernel->GetlpEngine() || !glpKernel->GetlpEngine()->GetlpGraphicsSystem() || !glpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer())
				{
					static_cast<LynxEngine::GameFramework::Android::CKernel*>(glpKernel)->SetApp(app);
				
					LynxEngine::GameFramework::CKernel::INIT_DESC Desc;
					Desc.GraphicParam.Height = ANativeWindow_getHeight(app->window) > MAX_SCREEN_HEIGHT ? MAX_SCREEN_HEIGHT : ANativeWindow_getHeight(app->window);
					Desc.GraphicParam.Width =  (int)(ANativeWindow_getWidth(app->window) * Desc.GraphicParam.Height / (float)ANativeWindow_getHeight(app->window));
					Desc.GraphicParam.DepthBufferFormat = LynxEngine::GraphicsSystem::CRenderer::FORMAT_D24S8;
					Desc.GraphicParam.NumMultiSamples = 1;
					Desc.GraphicParam.RendererType = LynxEngine::GraphicsSystem::CRenderer::OGLES2_RENDERER;
					glpKernel->m_InputScaleFactor = Desc.GraphicParam.Height / (float)ANativeWindow_getHeight(app->window);

					glpKernel->vInit(0, NULL, &Desc);
					gbInit = LYNX_TRUE;
				}
				else
				{
					glpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->Resume(app->window);
				}
			}
			break;

        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
			glpKernel->vRelease();
			break;

        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            /*
			if (engine->accelerometerSensor != NULL) 
			{
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                        engine->accelerometerSensor, (1000L/60)*1000);
            }
			*/
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            /*
			if (engine->accelerometerSensor != NULL) 
			{
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
            }
			*/
            break;
		case APP_CMD_START:
			/**
			 * Command from main thread: the app's activity has been started.
			 */
			break;
		case APP_CMD_RESUME:
			/**
			 * Command from main thread: the app's activity has been resumed.
			 */
			/*if (glpKernel->GetlpEngine() && glpKernel->GetlpEngine()->GetlpGraphicsSystem() && glpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer())
				glpKernel->GetlpEngine()->GetlpGraphicsSystem()->GetlpRenderer()->Resume();*/
			if (glpKernel->GetlpGame())
				glpKernel->GetlpGame()->vResume();
			break;
		case APP_CMD_PAUSE:
			/**
			 * Command from main thread: the app's activity has been paused.
			 */
			if (glpKernel->GetlpGame())
				glpKernel->GetlpGame()->vPause();
			break;
		case APP_CMD_STOP:
			/**
			 * Command from main thread: the app's activity has been stopped.
			 */
			if (glpKernel->GetlpGame())
				glpKernel->GetlpGame()->vTerminate(); 
			break;
		case APP_CMD_DESTROY:
			/**
			 * Command from main thread: the app's activity is being destroyed,
			 * and waiting for the app thread to clean up and exit before proceeding.
			 */
			break;
    }
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) 
{
    return 0;
}
//-------------------------------------------------------------------------------------------------------
//
//  說明:   
//-------------------------------------------------------------------------------------------------------
void android_main(struct android_app* state) 
{
    app_dummy();

	lynxInit(LYNX_TRUE);
	gbInit = LYNX_FALSE;
	
    state->userData = glpKernel;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    	
	while (1) 
	{
		int ident;
        int events;
        struct android_poll_source* source;

		while ((ident=ALooper_pollAll(0, NULL, &events,(void**)&source)) >= 0) 
		{
			// Process this event.
            if (source != NULL) 
			{
                source->process(state, source);
            }

			// Check if we are exiting.
            if (state->destroyRequested != 0) 
			{
                break;
			}
		}
		if (gbInit)
			glpKernel->vLoop();
	}

	/*
	while (1) 
	{
        // Read all pending events.
		int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events, (void**)&source)) >= 0) 
		{
            // Process this event.
            if (source != NULL) 
			{
                source->process(state, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) 
			{
                if (engine.accelerometerSensor != NULL) 
				{
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue, &event, 1) > 0) 
					{
                        LOGI("accelerometer: x=%f y=%f z=%f", event.acceleration.x, event.acceleration.y, event.acceleration.z);
                    }
                }
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) 
			{
                engine_term_display(&engine);
                return;
            }			
        }
		// Drawing is throttled to the screen update rate, so there
		// is no need to do timing here.
		engine_draw_frame(&engine);    		
    }
	*/

	lynxRelease();            
}
//END_INCLUDE(all)
