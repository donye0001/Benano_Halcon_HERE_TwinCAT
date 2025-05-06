//#pragma once
//#include <iostream>
//#include <string>
//#include <vector>
//
//namespace tpp
//{
//    //class HcLetter
//    //{
//    //public:
//    //    std::vector<char> hcLetter;  
//    //    char tx;// 傳送
//
//    //    char rx;// 接收
//
//    //    void serialize()
//    //    {
//    //        letter.clear();
//    //        appendToLetter(&tx, sizeof(tx));
//    //        appendToLetter(&moveMode, sizeof(moveMode));
//    //        appendToLetter(&position, sizeof(position));
//    //    }
//
//    //    void deserialize(void* src, size_t size)
//    //    {
//    //        if (size != this->callSize())
//    //        {
//    //            std::cout << size << std::endl;
//    //            std::cout << this->getSize() << std::endl;
//    //            throw("[TCP] Wrong src's size for deserialization.");
//    //        }
//
//    //        // 依對應成員的型態選擇不同的*(type*)方式容易打錯，改用memcpy與位元組讀取計數
//    //        size_t count = 0;
//    //        memcpy(&this->rx, (unsigned char*)src + count, sizeof(this->rx));
//    //        count += sizeof(this->rx);
//    //        memcpy(&this->fullTime, (unsigned char*)src + count, sizeof(this->fullTime));
//    //        count += sizeof(this->fullTime);
//    //        memcpy(&this->coeffCopy, (unsigned char*)src + count, sizeof(this->coeffCopy));
//    //    }
//
//    //    // 提供只讀訪問方式
//    //    const char* getData() const
//    //    {
//    //        return letter.data();
//    //    }
//
//    //    size_t getSize()
//    //    {
//    //        return letter.size();
//    //    }
//
//    //    size_t callSize()
//    //    {
//    //        return sizeof(rx) + sizeof(fullTime) + sizeof(coeffCopy);
//    //    }
//
//    //private:
//    //    // 輔助函數：將任意型別的資料附加到字節向量
//    //    template <typename T>
//    //    void appendToLetter(const T* data, size_t size)
//    //    {
//    //        const char* bytePtr = reinterpret_cast<const char*>(data);
//    //        letter.insert(letter.end(), bytePtr, bytePtr + size);
//    //    }
//    //};
//
//    class TcLetter
//    {
//    public:
//        std::vector<char> tcLetter;
//        char tx;// 傳送
//        int moveMode;
//        double position[6];
//        char rx;// 接收
//        double fullTime;
//        double coeffCopy[6][6];
//
//        void serialize()
//        {
//            tcLetter.clear();
//            appendToLetter(&tx, sizeof(tx));
//            appendToLetter(&moveMode, sizeof(moveMode));
//            appendToLetter(&position, sizeof(position));
//        }
//
//        void deserialize(void* src, size_t size)
//        {
//            if (size != this->callSize())
//            {
//                std::cout << size << std::endl;
//                std::cout << this->getSize() << std::endl;
//                throw("[TCP] Wrong src's size for deserialization from TwinCAT.");
//            }
//
//            // 依對應成員的型態選擇不同的*(type*)方式容易打錯，改用memcpy與位元組讀取計數
//            size_t count = 0;
//            memcpy(&this->rx, (unsigned char*)src + count, sizeof(this->rx));
//            count += sizeof(this->rx);
//            memcpy(&this->fullTime, (unsigned char*)src + count, sizeof(this->fullTime));
//            count += sizeof(this->fullTime);
//            memcpy(&this->coeffCopy, (unsigned char*)src + count, sizeof(this->coeffCopy));
//        }
//
//        // 提供只讀訪問方式
//        const char* getData() const
//        {
//            return tcLetter.data();
//        }
//
//        size_t getSize()
//        {
//            return tcLetter.size();
//        }
//
//        size_t callSize() // 接收的大小
//        {
//            return sizeof(rx) + sizeof(fullTime) + sizeof(coeffCopy);
//        }
//
//    private:
//        // 輔助函數：將任意型別的資料附加到字節向量
//        template <typename T>
//        void appendToLetter(const T* data, size_t size)
//        {
//            const char* bytePtr = reinterpret_cast<const char*>(data);
//            tcLetter.insert(tcLetter.end(), bytePtr, bytePtr + size);
//        }
//    };
//
//    class GzLetter
//    {
//    public:
//        std::vector<char> gzLetter;
//        char tx;// 傳送
//        double fullTime;
//        double coeffCopy[6][6];
//        char rx;// 接收
//        bool isOK;
//
//        void serialize()
//        {
//            gzLetter.clear();
//            appendToLetter(&tx, sizeof(tx));
//            appendToLetter(&fullTime, sizeof(fullTime));
//            appendToLetter(&coeffCopy, sizeof(coeffCopy));
//        }
//
//        void deserialize(void* src, size_t size)
//        {
//            if (size != this->callSize())
//            {
//                std::cout << size << std::endl;
//                std::cout << this->getSize() << std::endl;
//                throw("[TCP] Wrong src's size for deserialization from Gazebo.");
//            }
//
//            // 依對應成員的型態選擇不同的*(type*)方式容易打錯，改用memcpy與位元組讀取計數
//            size_t count = 0;
//            memcpy(&this->rx, (unsigned char*)src + count, sizeof(this->rx));
//            count += sizeof(this->rx);
//            memcpy(&this->isOK, (unsigned char*)src + count, sizeof(this->isOK));
//        }
//
//        // 提供只讀訪問方式
//        const char* getData() const
//        {
//            return gzLetter.data();
//        }
//
//        size_t getSize()
//        {
//            return gzLetter.size();
//        }
//
//        size_t callSize()
//        {
//            return sizeof(rx) + sizeof(isOK);
//        }
//
//    private:
//        // 輔助函數：將任意型別的資料附加到字節向量
//        template <typename T>
//        void appendToLetter(const T* data, size_t size)
//        {
//            const char* bytePtr = reinterpret_cast<const char*>(data);
//            gzLetter.insert(gzLetter.end(), bytePtr, bytePtr + size);
//        }
//    };
//}
