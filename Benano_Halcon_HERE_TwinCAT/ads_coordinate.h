#pragma once

namespace ads
{
    // �Ǫ��j���e�O�ϥΨӶǿ���|�Y�ơA�ڭ̫h�令�ǿ��m
    class Linear 
    {
    public:
        struct Request
        {
            bool   new_data;
            double target_postion[3];
            //double final_position[6];
        } req;

        struct Response
        {
            //double real_time_ang[6];
            bool   arrive;
        } resp;
    }; //class linear
} // namespace ads