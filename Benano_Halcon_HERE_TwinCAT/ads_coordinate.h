#pragma once

namespace ads
{
    // 學長姐之前是使用來傳輸路徑係數，我們則改成傳輸位置
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