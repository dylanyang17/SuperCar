#include "Kalman.h"


void Kalman_Filter(float Accel,        float Gyro ,KalmanCountData * Kalman_Struct)
{
     //�����ǻ��ֽǶȣ�������ƣ�
    Kalman_Struct -> Angle_Final += (Gyro - Kalman_Struct -> Q_bias) * dt; 
                
    //����������Э�����΢��
    Kalman_Struct -> Pdot[0] = Q_angle - Kalman_Struct -> PP[0][1] - Kalman_Struct -> PP[1][0]; 
    Kalman_Struct -> Pdot[1] = - Kalman_Struct -> PP[1][1];
    Kalman_Struct -> Pdot[2] = - Kalman_Struct -> PP[1][1];
    Kalman_Struct -> Pdot[3] = Q_gyro;
                
    //����������Э����Ļ���
    Kalman_Struct -> PP[0][0] += Kalman_Struct -> Pdot[0] * dt;   
    Kalman_Struct -> PP[0][1] += Kalman_Struct -> Pdot[1] * dt;   
    Kalman_Struct -> PP[1][0] += Kalman_Struct -> Pdot[2] * dt;
    Kalman_Struct -> PP[1][1] += Kalman_Struct -> Pdot[3] * dt;
                
    //����Ƕ�ƫ��
    Kalman_Struct -> Angle_err = Accel - Kalman_Struct -> Angle_Final;        
                
    //�������������
    Kalman_Struct -> PCt_0 = C_0 * Kalman_Struct -> PP[0][0];
    Kalman_Struct -> PCt_1 = C_0 * Kalman_Struct -> PP[1][0];
                
    Kalman_Struct -> E = R_angle + C_0 * Kalman_Struct -> PCt_0;
                
    Kalman_Struct -> K_0 = Kalman_Struct -> PCt_0 / Kalman_Struct -> E;
    Kalman_Struct -> K_1 = Kalman_Struct -> PCt_1 / Kalman_Struct -> E;
                
    //����������Э�������
    Kalman_Struct -> t_0 = Kalman_Struct -> PCt_0;
    Kalman_Struct -> t_1 = C_0 * Kalman_Struct -> PP[0][1];

    Kalman_Struct -> PP[0][0] -= Kalman_Struct -> K_0 * Kalman_Struct -> t_0;                 
    Kalman_Struct -> PP[0][1] -= Kalman_Struct -> K_0 * Kalman_Struct -> t_1;
    Kalman_Struct -> PP[1][0] -= Kalman_Struct -> K_1 * Kalman_Struct -> t_0;
    Kalman_Struct -> PP[1][1] -= Kalman_Struct -> K_1 * Kalman_Struct -> t_1;
		
		//����������ŽǶ�ֵ
    Kalman_Struct -> Angle_Final += Kalman_Struct -> K_0 * Kalman_Struct -> Angle_err;
    
		//�������Ź���ֵ��ƫ��
		Kalman_Struct -> Q_bias        += Kalman_Struct -> K_1 * Kalman_Struct -> Angle_err;
    
		//�������Ž��ٶ�ֵ
		Kalman_Struct -> Gyro_Final   = Gyro - Kalman_Struct -> Q_bias;
}

//Kalman Filter �м�����ʼ��
void Kalman_Filter_Init(KalmanCountData * Kalman_Struct)
{
        Kalman_Struct -> Angle_err                 = 0;
        Kalman_Struct -> Q_bias                         = 0;
        Kalman_Struct -> PCt_0                         = 0;
        Kalman_Struct -> PCt_1                         = 0;
        Kalman_Struct -> E                                 = 0;
        Kalman_Struct -> K_0                         = 0;
        Kalman_Struct -> K_1                         = 0;
        Kalman_Struct -> t_0                         = 0;
        Kalman_Struct -> t_1                         = 0;
        Kalman_Struct -> Pdot[0]                 = 0;
        Kalman_Struct -> Pdot[1]                 = 0;
        Kalman_Struct -> Pdot[2]                 = 0;
        Kalman_Struct -> Pdot[3]                 = 0;        
        Kalman_Struct -> PP[0][0]                 = 1;
        Kalman_Struct -> PP[0][1]                 = 0;
        Kalman_Struct -> PP[1][0]                 = 0;
        Kalman_Struct -> PP[1][1]                 = 1;        
        Kalman_Struct -> Angle_Final         = 0;
        Kalman_Struct -> Gyro_Final                 = 0;

}