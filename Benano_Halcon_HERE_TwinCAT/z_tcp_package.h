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
//    //    char tx;// �ǰe
//
//    //    char rx;// ����
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
//    //        // �̹������������A��ܤ��P��*(type*)�覡�e�������A���memcpy�P�줸��Ū���p��
//    //        size_t count = 0;
//    //        memcpy(&this->rx, (unsigned char*)src + count, sizeof(this->rx));
//    //        count += sizeof(this->rx);
//    //        memcpy(&this->fullTime, (unsigned char*)src + count, sizeof(this->fullTime));
//    //        count += sizeof(this->fullTime);
//    //        memcpy(&this->coeffCopy, (unsigned char*)src + count, sizeof(this->coeffCopy));
//    //    }
//
//    //    // ���ѥuŪ�X�ݤ覡
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
//    //    // ���U��ơG�N���N���O����ƪ��[��r�`�V�q
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
//        char tx;// �ǰe
//        int moveMode;
//        double position[6];
//        char rx;// ����
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
//            // �̹������������A��ܤ��P��*(type*)�覡�e�������A���memcpy�P�줸��Ū���p��
//            size_t count = 0;
//            memcpy(&this->rx, (unsigned char*)src + count, sizeof(this->rx));
//            count += sizeof(this->rx);
//            memcpy(&this->fullTime, (unsigned char*)src + count, sizeof(this->fullTime));
//            count += sizeof(this->fullTime);
//            memcpy(&this->coeffCopy, (unsigned char*)src + count, sizeof(this->coeffCopy));
//        }
//
//        // ���ѥuŪ�X�ݤ覡
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
//        size_t callSize() // �������j�p
//        {
//            return sizeof(rx) + sizeof(fullTime) + sizeof(coeffCopy);
//        }
//
//    private:
//        // ���U��ơG�N���N���O����ƪ��[��r�`�V�q
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
//        char tx;// �ǰe
//        double fullTime;
//        double coeffCopy[6][6];
//        char rx;// ����
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
//            // �̹������������A��ܤ��P��*(type*)�覡�e�������A���memcpy�P�줸��Ū���p��
//            size_t count = 0;
//            memcpy(&this->rx, (unsigned char*)src + count, sizeof(this->rx));
//            count += sizeof(this->rx);
//            memcpy(&this->isOK, (unsigned char*)src + count, sizeof(this->isOK));
//        }
//
//        // ���ѥuŪ�X�ݤ覡
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
//        // ���U��ơG�N���N���O����ƪ��[��r�`�V�q
//        template <typename T>
//        void appendToLetter(const T* data, size_t size)
//        {
//            const char* bytePtr = reinterpret_cast<const char*>(data);
//            gzLetter.insert(gzLetter.end(), bytePtr, bytePtr + size);
//        }
//    };
//}
