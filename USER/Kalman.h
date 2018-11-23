#ifndef __KALMAN_H__
#define __KALMAN_H__


#define Q_angle                        0.001       ////�Ƕȹ���������Э����
#define Q_gyro                        0.003        ////���ٶȹ���������Э����
#define R_angle                        0.05                //����������Э����
#define dt                                0.01            //����Ƶ��
#define C_0                                1



typedef struct
{
        float                                Q_bias;            //���Ź���ֵ��ƫ��
        float                                Angle_err;         //ʵ��Ƕ��������ǻ��ֽǶȵĲ�ֵ
        float                                PCt_0;                                
        float                                PCt_1; 
        float                                E;                 //����Ĺ�����
        float                                K_0;               //���п��������������һ������
        float                                K_1;                //���п���������ĺ��������ڼ������Ź���ֵ��ƫ��
        float                                t_0;                                
        float                                t_1;
        float                                Pdot[4];            //����Э��������΢�־���
        float                                PP[2][2];           //Э���� covariance
        float                                Angle_Final;        //����������ŽǶ�ֵ
        float                                Gyro_Final;        //����������Ž��ٶ�ֵ

}KalmanCountData;

void Kalman_Filter(float Accel,        float Gyro ,KalmanCountData * Kalman_Struct);
void Kalman_Filter_Init(KalmanCountData * Kalman_Struct);	//Initiate Kalman Filter
#endif
