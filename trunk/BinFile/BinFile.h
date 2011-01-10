class BinFile
{
    public:
        BinFile ();
        virtual ~BinFile ();
        int Add();
        void setX(int x);
        void setY(int y);

    private:
        int x;                                    //Data member
        int y;                                    // Data member
		std::vector <int> mData(bufWidth*4);
};
