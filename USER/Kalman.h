#ifndef __KALMAN_H__
#define __KALMAN_H__


#define Q_angle                        0.001       ////角度过程噪声的协方差
#define Q_gyro                        0.003        ////角速度过程噪声的协方差
#define R_angle                        0.5                //测量噪声的协方差
#define dt                                0.01            //采样频率
#define C_0                                1



typedef struct
{
        float                                Q_bias;            //最优估计值的偏差
        float                                Angle_err;         //实测角度与陀螺仪积分角度的差值
        float                                PCt_0;                                
        float                                PCt_1; 
        float                                E;                 //计算的过程量
        float                                K_0;               //含有卡尔曼增益的另外一个函数
        float                                K_1;                //含有卡尔曼增益的函数，用于计算最优估计值的偏差
        float                                t_0;                                
        float                                t_1;
        float                                Pdot[4];            //过程协方差矩阵的微分矩阵
        float                                PP[2][2];           //协方差 covariance
        float                                Angle_Final;        //后验估计最优角度值
        float                                Gyro_Final;        //后验估计最优角速度值

}KalmanCountData;

extern void Kalman_Filter(float Accel,        float Gyro ,KalmanCountData * Kalman_Struct);
extern void Kalman_Filter_Init(KalmanCountData * Kalman_Struct);	//Initiate Kalman Filter
#endif
