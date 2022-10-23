
#include <app/app_api.h>
#include <app/app_global.h>
#include <scheduling/scheduler.h>
#include <scheduling/scheduler_based_task.h>


__SHARED_VAR(
uint16_t        i_debug;
uint16_t        ar_v_seed;

uint16_t        _v_count;
uint16_t        _v_pinState;
uint16_t        _v_samplesInWindow;
uint16_t        _v_totalCount;
uint16_t        _v_movingCount;
uint16_t        _v_stationaryCount;
uint16_t        _v_discardedSamplesCount;
uint16_t        _v_trainingSetSize;

ar_class_t      _v_class;
ar_run_mode_t   _v_mode;
ar_features_t   _v_features;
uint16_t        _v_meanmag;
uint16_t        _v_stddevmag;
accelReading    _v_window[AR_ACCEL_WINDOW_SIZE];
ar_features_t   _v_model_stationary[AR_MODEL_SIZE];
ar_features_t   _v_model_moving[AR_MODEL_SIZE];

uint16_t        resultStationaryPct[4];
uint16_t        resultMovingPct[4];
uint16_t        sum[4];
)

// //to .h
// #define vbm_test(v) v == cur_version
// #define vbm_set(v) v = cur_version

// declaration
__nv uint16_t _v_pinState_priv;
__nv uint16_t _v_discardedSamplesCount_priv;
//__nv ar_class_t _v_class_priv;//6   // 4  

__nv uint16_t _v_totalCount_priv;
__nv uint16_t _v_movingCount_priv;
__nv uint16_t _v_stationaryCount_priv;//12   // 7   

__nv accelReading _v_window_priv[ACCEL_WINDOW_SIZE];  //23   
__nv accelReading _v_window_vbm[ACCEL_WINDOW_SIZE];

//__nv ar_features_t _v_features_priv;//25   // 18   

__nv uint16_t _v_trainingSetSize_priv;//27  

__nv uint16_t _v_samplesInWindow_priv;
//__nv ar_run_mode_t _v_mode_priv;  //31 

__nv uint16_t _v_count_priv;



__TASK_ENTRY(init,

__GET(i_debug) = 0;
__GET(ar_v_seed) = 0x1111;
__GET(_v_pinState) = MODE_IDLE;
__GET(_v_count) = 0;

return 1;
)

__TASK(1, Select_Mode,

    //1.
_v_pinState_priv = __GET(_v_pinState);
_v_count_priv = __GET(_v_count);

//_v_discardedSamplesCount_priv = __GET(_v_discardedSamplesCount);
//_v_class_priv = __GET(_v_class);//6   // 4  

//_v_totalCount_priv = __GET(_v_totalCount);
//_v_movingCount_priv = __GET(_v_movingCount);
//_v_stationaryCount_priv = __GET(_v_stationaryCount);//12   // 7   

//_v_window_priv[ACCEL_WINDOW_SIZE];  //23   

//_v_features_priv = __GET(_v_features);//25   // 18   

//_v_trainingSetSize_priv = __GET(_v_trainingSetSize);//27  

 //_v_samplesInWindow_priv = __GET(_v_samplesInWindow);
 //_v_mode_priv = __GET(_v_mode);  //31 


       volatile uint16_t pin_state = MODE_TRAIN_MOVING;  // 1
       __GET(_v_count)++;

       if(__GET(_v_count) >= 7)             return TASK_FINISH;
       else if(__GET(_v_count) >= 3)       pin_state = MODE_RECOGNIZE;        // 0
       else if(__GET(_v_count) >= 2)       pin_state = MODE_TRAIN_STATIONARY; // 2

       if ( (pin_state == MODE_TRAIN_STATIONARY || pin_state == MODE_TRAIN_MOVING)
               && (pin_state == _v_pinState_priv) ) {  
           pin_state = MODE_IDLE;
       }
       else {
           _v_pinState_priv = pin_state;
       }


       switch(pin_state)
       {
       case MODE_TRAIN_STATIONARY:
           __GET(_v_discardedSamplesCount) = 0;
           __GET(_v_mode) = MODE_TRAIN_STATIONARY;
           __GET(_v_class) = CLASS_STATIONARY;
           __GET(_v_samplesInWindow) = 0;

           //3
           write_to_gbuf(&_v_pinState_priv, &_v_pinState, sizeof(_v_pinState));
           //write_to_gbuf(&_v_discardedSamplesCount_priv, &_v_discardedSamplesCount, sizeof(_v_discardedSamplesCount));
           //write_to_gbuf(&_v_class_priv, &_v_class_priv, sizeof(_v_class));
  /*         write_to_gbuf(&_v_totalcount_priv, &_v_totalcount, sizeof(_v_totalcount));
           write_to_gbuf(&_v_movingcount_priv, &_v_movingcount, sizeof(_v_movingcount));
           write_to_gbuf(&_v_stationarycount_priv, &_v_stationarycount, sizeof(_v_stationarycount));
           write_to_gbuf(&_v_features_priv, &_v_features, sizeof(_v_features));
           write_to_gbuf(&_v_trainingsetsize_priv, &_v_trainingsetsize, sizeof(_v_trainingsetsize));
           write_to_gbuf(&_v_samplesinwindow_priv, &_v_samplesinwindow, sizeof(_v_samplesinwindow));*/
           write_to_gbuf(&_v_count_priv, &_v_count, sizeof(_v_count));

           return 7;

       case MODE_TRAIN_MOVING:
           __GET(_v_discardedSamplesCount) = 0;
           __GET(_v_mode) = MODE_TRAIN_MOVING;
           __GET(_v_class) = CLASS_MOVING;
           __GET(_v_samplesInWindow) = 0;

           //3
           write_to_gbuf(&_v_pinState_priv, &_v_pinState, sizeof(_v_pinState));
           write_to_gbuf(&_v_count_priv, &_v_count, sizeof(_v_count));

           return 7;

       case MODE_RECOGNIZE:
           __GET(_v_mode) = MODE_RECOGNIZE;
           __GET(_v_movingCount) = 0;
           __GET(_v_stationaryCount) = 0;
           __GET(_v_totalCount) = 0;
           __GET(_v_samplesInWindow) = 0;

           //3
           write_to_gbuf(&_v_pinState_priv, &_v_pinState, sizeof(_v_pinState));
           write_to_gbuf(&_v_count_priv, &_v_count, sizeof(_v_count));
           return 2;

       default: 
           //3
           write_to_gbuf(&_v_pinState_priv, &_v_pinState, sizeof(_v_pinState));
           write_to_gbuf(&_v_count_priv, &_v_count, sizeof(_v_count));
           return 1;
       }
)

__TASK(2, AR_Sample,

    //1.
    _v_samplesInWindow_priv = __GET(_v_samplesInWindow);

       accelReading    aR_sample;
       AR_SingleSample(&aR_sample, &(__GET(ar_v_seed)));

       __GET(_v_window[_v_samplesInWindow_priv].x) = (aR_sample).x;
       __GET(_v_window[_v_samplesInWindow_priv].y) = (aR_sample).y;
       __GET(_v_window[_v_samplesInWindow_priv].z) = (aR_sample).z;
       ++_v_samplesInWindow_priv;

       if (_v_samplesInWindow_priv < AR_ACCEL_WINDOW_SIZE) {
           //3
           write_to_gbuf(&_v_samplesInWindow_priv, &_v_samplesInWindow, sizeof(_v_samplesInWindow));
           return 2;
       }
       else {
           _v_samplesInWindow_priv = 0;
           //3
           write_to_gbuf(&_v_samplesInWindow_priv, &_v_samplesInWindow, sizeof(_v_samplesInWindow));
           return 3;
       }
)

__TASK(3, AR_Transform,  //_v_window[]

       for (uint16_t i = 0; i < AR_ACCEL_WINDOW_SIZE; ++i)
       {

           //before read 
           if (!vbm_test(_v_window_vbm[i])) {
               _v_window_priv[i] = __GET(_v_window[i]);
           }
  /*         if (!vbm_test(_v_window_vbm[i].y)) {
               _v_window_priv[i].y = __GET(_v_window[i].y;
           }
           if (!vbm_test(_v_window_vbm[i].z)) {
               _v_window_priv[i].z = __GET(_v_window[i].z;
           }*/

           if (__GET(_v_window_priv[i].x) < AR_SAMPLE_NOISE_FLOOR ||
                   __GET(_v_window_priv[i].y) < AR_SAMPLE_NOISE_FLOOR ||
                         __GET(_v_window_priv[i].z) < AR_SAMPLE_NOISE_FLOOR)
           {
               __GET(_v_window_priv[i].x) =
                       __GET(_v_window_priv[i].x) > AR_SAMPLE_NOISE_FLOOR ? __GET(_v_window_priv[i].x) : 0;
               __GET(_v_window_priv[i].y) =
                       __GET(_v_window_priv[i].y) > AR_SAMPLE_NOISE_FLOOR ? __GET(_v_window_priv[i].y) : 0;
               __GET(_v_window_priv[i].z) =
                       __GET(_v_window_priv[i].z) > AR_SAMPLE_NOISE_FLOOR ? __GET(_v_window_priv[i].z) : 0;
               //after wt
               if (!vbm_test(_v_window_vbm[i])) {
                   vbm_set(_v_window_vbm[i]);
                   write_to_gbuf(&_v_window_priv[i], &_v_window[i], sizeof(_v_window[i]));
               }
           }
       }
return 4;

)

__TASK(4, AR_Featurize,

       accelReading mean;
       accelReading stddev;
       ar_features_t features;
       accelReading aR_sample;

       mean.x = mean.y = mean.z = 0;
       stddev.x = stddev.y = stddev.z = 0;

       for (uint16_t i = 0; i < AR_ACCEL_WINDOW_SIZE; ++i)
       {
           mean.x += __GET(_v_window[i].x);
           mean.y += __GET(_v_window[i].y);
           mean.z += __GET(_v_window[i].z);
       }

       mean.x >>= 2;
       mean.y >>= 2;
       mean.z >>= 2;

       for (uint16_t i = 0; i < AR_ACCEL_WINDOW_SIZE; ++i)
       {
           (aR_sample).x = __GET(_v_window[i].x);
           (aR_sample).y = __GET(_v_window[i].y);
           (aR_sample).z = __GET(_v_window[i].z);

           stddev.x += ((aR_sample).x > mean.x)? ((aR_sample).x - mean.x): (mean.x - (aR_sample).x);
           stddev.y += ((aR_sample).y > mean.y)? ((aR_sample).y - mean.y): (mean.y - (aR_sample).y);
           stddev.z += ((aR_sample).z > mean.z)? ((aR_sample).z - mean.z): (mean.z - (aR_sample).z);

       }

       stddev.x >>= 2;
       stddev.y >>= 2;
       stddev.z >>= 2;

       __GET(_v_meanmag) = mean.x * mean.x + mean.y * mean.y + mean.z * mean.z;
       __GET(_v_stddevmag) = stddev.x * stddev.x + stddev.y * stddev.y + stddev.z * stddev.z;

       features.meanmag = sqrt16(__GET(_v_meanmag));
       features.stddevmag = sqrt16(__GET(_v_stddevmag));

       switch (__GET(_v_mode))
       {
       case MODE_TRAIN_STATIONARY:
       case MODE_TRAIN_MOVING:
           __GET(_v_features.meanmag) = features.meanmag;
           __GET(_v_features.stddevmag) = features.stddevmag;
           return 8;
       case MODE_RECOGNIZE:
           __GET(_v_features.meanmag) = features.meanmag;
           __GET(_v_features.stddevmag) = features.stddevmag;
           return 5;
       }
)

__TASK(5, AR_Classify,

       uint16_t move_less_error = 0;
       uint16_t stat_less_error = 0;
       uint32_t l_meanmag = __GET(_v_features.meanmag);
       uint32_t l_stddevmag = __GET(_v_features.stddevmag);

       ar_features_t ms, mm;

       for (uint16_t i = 0; i < AR_MODEL_SIZE; ++i)
       {
           ms.meanmag = __GET(_v_model_stationary[i].meanmag);
           ms.stddevmag = __GET(_v_model_stationary[i].stddevmag);
           mm.meanmag = __GET(_v_model_moving[i].meanmag);
           mm.stddevmag = __GET(_v_model_moving[i].stddevmag);

           int32_t stat_mean_err = (ms.meanmag > l_meanmag)
               ? (ms.meanmag - l_meanmag)
               : (l_meanmag - ms.meanmag);

           int32_t stat_sd_err = (ms.stddevmag > l_stddevmag)
               ? (ms.stddevmag - l_stddevmag)
               : (l_stddevmag - ms.stddevmag);

           int32_t move_mean_err = (mm.meanmag > l_meanmag)
               ? (mm.meanmag - l_meanmag)
               : (l_meanmag - mm.meanmag);

           int32_t move_sd_err = (mm.stddevmag > l_stddevmag)
               ? (mm.stddevmag - l_stddevmag)
               : (l_stddevmag - mm.stddevmag);

           if (move_mean_err < stat_mean_err)
               move_less_error++;
           else
               stat_less_error++;

           if (move_sd_err < stat_sd_err)
               move_less_error++;
           else
               stat_less_error++;
       }

       __GET(_v_class) = (move_less_error > stat_less_error) ? CLASS_MOVING : CLASS_STATIONARY;
       return 6;
)

__TASK(6, AR_Stats,

    //1.
    _v_totalCount_priv = __GET(_v_totalCount);
_v_movingCount_priv = __GET(_v_movingCount);
_v_stationaryCount_priv = __GET(_v_stationaryCount);

       _v_totalCount_priv++;
       switch(__GET(_v_class))
       {
       case CLASS_MOVING:
           _v_movingCount_priv++;
           break;
       case CLASS_STATIONARY:
           _v_stationaryCount_priv++;
           break;
       }

       if (_v_totalCount_priv == AR_SAMPLES_TO_COLLECT)
       {
           __GET(resultStationaryPct[__GET(i_debug)]) =
                   _v_stationaryCount_priv * 100 / _v_totalCount_priv;

           __GET(resultMovingPct[__GET(i_debug)]) =
                   _v_movingCount_priv * 100 / _v_totalCount_priv;

           __GET(sum[__GET(i_debug)]) =
                   _v_stationaryCount_priv + _v_movingCount_priv;

           __GET(i_debug)++;

           //3
           write_to_gbuf(&_v_totalCount_priv, &_v_totalCount, sizeof(_v_totalCount));
           write_to_gbuf(&_v_movingCount_priv, &_v_movingCount, sizeof(_v_movingCount));
           write_to_gbuf(&_v_stationaryCount_priv, &_v_stationaryCount, sizeof(_v_stationaryCount));

           return 1;
       }
       else {
           //3
           write_to_gbuf(&_v_totalCount_priv, &_v_totalCount, sizeof(_v_totalCount));
           write_to_gbuf(&_v_movingCount_priv, &_v_movingCount, sizeof(_v_movingCount));
           write_to_gbuf(&_v_stationaryCount_priv, &_v_stationaryCount, sizeof(_v_stationaryCount));

           return 2;
       }
)

__TASK(7, Warm_Up,
    //1.
    _v_discardedSamplesCount_priv = __GET(_v_discardedSamplesCount);

       threeAxis_t_8 tA8_sample;
       if (__GET(_v_discardedSamplesCount) < AR_NUM_WARMUP_SAMPLES)
       {
           AR_SingleSample(&tA8_sample, &(__GET(ar_v_seed)));
           __GET(_v_discardedSamplesCount)++;
           //3
           write_to_gbuf(&_v_discardedSamplesCount_priv, &_v_discardedSamplesCount, sizeof(_v_discardedSamplesCount));
           return 7;
       }
       else
       {
           __GET(_v_trainingSetSize) = 0;
           //3
           write_to_gbuf(&_v_discardedSamplesCount_priv, &_v_discardedSamplesCount, sizeof(_v_discardedSamplesCount));
           return 2;
       }
)

__TASK(8, AR_Train,

    //1.
    _v_trainingSetSize_priv = __GET(_v_trainingSetSize);//

       switch(__GET(_v_class))
       {
       case CLASS_STATIONARY:
           __GET(_v_model_stationary[_v_trainingSetSize_priv].meanmag) =
                   __GET(_v_features.meanmag);
           __GET(_v_model_stationary[_v_trainingSetSize_priv].stddevmag) =
                   __GET(_v_features.stddevmag);
           break;
       case CLASS_MOVING:
           __GET(_v_model_moving[_v_trainingSetSize_priv].meanmag) =
                   __GET(_v_features.meanmag);
           __GET(_v_model_moving[_v_trainingSetSize_priv].stddevmag) =
                   __GET(_v_features.stddevmag);
           break;
       }

       _v_trainingSetSize_priv++;
       if(_v_trainingSetSize_priv < AR_MODEL_SIZE) {
           //3
           write_to_gbuf(&_v_trainingSetSize_priv, &_v_trainingSetSize, sizeof(_v_trainingSetSize));

           return 2;
       }
       else {
           //3
           write_to_gbuf(&_v_trainingSetSize_priv, &_v_trainingSetSize, sizeof(_v_trainingSetSize));

           return 1;
       }
)


void ar_regist()
{
    task_regist(0, init,            false);
    task_regist(1, Select_Mode,     true );
    task_regist(2, AR_Sample,       true );
    task_regist(3, AR_Transform,    false);
    task_regist(4, AR_Featurize,    false);
    task_regist(5, AR_Classify,     false);
    task_regist(6, AR_Stats,        true );
    task_regist(7, Warm_Up,         true );
    task_regist(8, AR_Train,        true );

    WAR_REGIST(10);
}

