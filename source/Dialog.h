#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

struct btn_cadre {
    int buttonId;
    int x1;
    int y1;
    int x2;
    int y2;
};

class Dialog {
    private:
        static const int marge = 10;
        static constexpr float fontSize = 0.5f;
        vector<btn_cadre> btn_cadres;
        
        static void getStringDimensions(const string& string, float& outWidth, float& outHeight);
        static float getTextWidth(const C2D_Text& text);
        static float getTextHeight(const C2D_Text& text);
        static float getStringWidth(const string& string);
        static float getStringHeight(const string& string);
        static void split(const string& text, char delimiter, vector<string>& result);

    public:
        void ask(const string& text, vector<string>& buttons);
        int hasUserClickedOnBtn(touchPosition& touch);
};