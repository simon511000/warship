#include "3ds.h"
#include "citro2d.h"
#include "Dialog.h"

void Dialog::ask(const string& text, vector<string>& buttons) {
    //Largeur maximale de la boîte de dialogue : 4/5 de la largeur de la fenêtre
    int w_max = (GSP_SCREEN_WIDTH/5)*4;
    
    C2D_TextBuf textBuf = C2D_TextBufNew(4096);
    vector<string> message_list;

    // Largeur max des textes
    int w_t = 0;
    
    if(Dialog::getStringWidth(text) > w_max) {
        // Il faut découper le message !

        // Découpe du message
        vector<string> mots;
        Dialog::split(text, ' ', mots);

        // Construction des découpes
        string s = "";
        for(string& mot: mots) {
            float sWidth = Dialog::getStringWidth(s);
            if((sWidth + Dialog::getStringWidth(mot)) <= w_max) {
                if(s == "") {
                    s = mot;
                } else {
                    s += " " + mot;
                }
            } else {
                if(sWidth > w_t)
                    w_t = sWidth;

                message_list.push_back(s);
                s = mot;
            }
        }
        if(s != "") {
            message_list.push_back(s);
        }

    } else {
        message_list.push_back(text);
        w_t = Dialog::getStringWidth(text);
    }

    w_t += 2*Dialog::marge;

    // Création des boutons et Calcul de la largeur totale des boîtes des boutons
    vector<C2D_Text> buttonsText;
    int w_buttons = Dialog::marge*buttons.size();
    for(string& button: buttons) {
        C2D_Text text;
        C2D_TextParse(&text, textBuf, button.c_str());
        C2D_TextOptimize(&text);
        buttonsText.push_back(text);

        w_buttons += getTextWidth(text) + 2*Dialog::marge;
    }

    vector<vector<C2D_Text>> button_list;
    if(w_buttons > w_max) {
        // Il faut découper les boutons sur plusieurs lignes
        float w = 0;
        vector<C2D_Text> lst;
        for(C2D_Text button: buttonsText) {
            float buttonWidth = Dialog::getTextWidth(button) + 3*Dialog::marge;
            if((w + buttonWidth) < w_max) {
                lst.push_back(button);
                w += buttonWidth;
            } else {
                button_list.push_back(lst);
                w = buttonWidth;
                lst.clear();
                lst.push_back(button);
            }
        }
        button_list.push_back(lst);
    } else {
        button_list.push_back(buttonsText);
    }

    // Calcul de la largeur et hauteur max des boutons
    float w_b = 0;
    float h_b = 0;
    for(vector<C2D_Text>& lst: button_list) {
        float _w_b = Dialog::marge * (lst.size()+1);

        for(C2D_Text& button: lst) {
            float outWidth;
            float outHeight;
            C2D_TextGetDimensions(&button, Dialog::fontSize, Dialog::fontSize, &outWidth, &outHeight);

            _w_b += outWidth;

            if(outHeight > h_b)
                h_b = outHeight;
        }

        if(_w_b > w_b)
            w_b = _w_b;
    }

    // Largeur de la boîte de dialogue
    float w_boite = w_t > w_b ? w_t : w_b;

    // Calcul de la hauteur de la boîte de dialogue
    int h_textes = 0;
    for(string& message: message_list) {
        h_textes += Dialog::getStringHeight(message);
    }
    int h_boite = h_textes+(Dialog::marge*2) + button_list.size()*(h_b+Dialog::marge*3);

    // On peut maintenant dessiner la boîte de dialogue
    int x_boite = (GSP_SCREEN_HEIGHT_BOTTOM/2)-(w_boite/2);
    int y_boite = (GSP_SCREEN_WIDTH/2)-(h_boite/2);

    // Dessin du fond de la boîte
    C2D_DrawRectSolid(x_boite, y_boite, 0.0f, w_boite, h_boite, C2D_Color32(255,255,255,255));

    // Dessin du cadre de la boîte
    // haut
    C2D_DrawLine(x_boite, y_boite, C2D_Color32(0,0,0,255), x_boite+w_boite, y_boite, C2D_Color32(0,0,0,255), 1, 0.0f);
    // droite
    C2D_DrawLine(x_boite+w_boite, y_boite, C2D_Color32(0,0,0,255), x_boite+w_boite, y_boite+h_boite, C2D_Color32(0,0,0,255), 1, 0.0f);
    // bas
    C2D_DrawLine(x_boite, y_boite+h_boite, C2D_Color32(0,0,0,255), x_boite+w_boite, y_boite+h_boite, C2D_Color32(0,0,0,255), 1, 0.0f);
    // gauche
    C2D_DrawLine(x_boite, y_boite, C2D_Color32(0,0,0,255), x_boite, y_boite+h_boite, C2D_Color32(0,0,0,255), 1, 0.0f);

    // Affichage du texte
    int y = y_boite + Dialog::marge;
    for (int i = 0; i < (int)message_list.size(); i++) {
        C2D_Text text;
        C2D_TextParse(&text, textBuf, message_list[i].c_str());
        C2D_TextOptimize(&text);
        C2D_DrawText(&text, C2D_AlignLeft, x_boite + Dialog::marge, y, 0.0f, Dialog::fontSize, Dialog::fontSize);

        y += getTextHeight(text);
    }

    // Stockage des cadres des boutons
    vector<btn_cadre> btn_cadres;

    // Affichage des boutons
    y += Dialog::marge;
    int buttonId = 0;
    for(vector<C2D_Text>& lst: button_list) {
        float w = 3 * lst.size() * Dialog::marge;
        for(C2D_Text& button: lst) {
            w += Dialog::getTextWidth(button);
        }
        int space = (w_boite - w) / 2;
        int x = space + x_boite;
        for(C2D_Text& button: lst) {
            // Calcul de la largeur du bouton
            float w_button = Dialog::getTextWidth(button) + 2*Dialog::marge;
            // Calcul de la hauteur du bouton
            float h_button = Dialog::getTextHeight(button) + 2*Dialog::marge;

            // Ajout du cadre du bouton
            btn_cadres.push_back((btn_cadre){buttonId, x, y, (int)w_button+x, (int)h_button+y});
            
            // Dessin du fond du bouton
            C2D_DrawRectSolid(x, y, 0.0f, w_button, h_button, C2D_Color32(192, 192, 192, 255));

            // Dessin du cadre du bouton
            // haut
            C2D_DrawLine(x, y, C2D_Color32(0,0,0,255), x+w_button, y, C2D_Color32(0,0,0,255), 1, 0.0f);
            // droite
            C2D_DrawLine(x+w_button, y, C2D_Color32(0,0,0,255), x+w_button, y+h_button, C2D_Color32(0,0,0,255), 1, 0.0f);
            // bas
            C2D_DrawLine(x, y+h_button, C2D_Color32(0,0,0,255), x+w_button, y+h_button, C2D_Color32(0,0,0,255), 1, 0.0f);
            // gauche
            C2D_DrawLine(x, y, C2D_Color32(0,0,0,255), x, y+h_button, C2D_Color32(0,0,0,255), 1, 0.0f);

            C2D_DrawText(&button, C2D_AlignLeft, x+Dialog::marge, y+Dialog::marge, 1.0f, Dialog::fontSize, Dialog::fontSize);

            x += w_button + Dialog::marge;
            buttonId++;
        }
        y += 3*Dialog::marge + h_b;
    }

    this->btn_cadres = btn_cadres;
    
    C2D_TextBufDelete(textBuf);   
}

int Dialog::hasUserClickedOnBtn(touchPosition& touch) {
    for(btn_cadre& btn: this->btn_cadres) {
        if((btn.x1 <= touch.px) && (touch.px <= btn.x2) && (btn.y1 <= touch.py) && (touch.py <= btn.y2)) {
            return btn.buttonId;
        }
    }
    return -1;
}

void Dialog::getStringDimensions(const string& string, float& outWidth, float& outHeight) {
    C2D_TextBuf textBuf = C2D_TextBufNew(4096);
    C2D_Text text;
    C2D_TextParse(&text, textBuf, string.c_str());

    C2D_TextGetDimensions(&text, Dialog::fontSize, Dialog::fontSize, &outWidth, &outHeight);
    C2D_TextBufDelete(textBuf);
}

float Dialog::getTextWidth(const C2D_Text& text) {
    float outWidth;
    float outHeight;
    C2D_TextGetDimensions(&text, Dialog::fontSize, Dialog::fontSize, &outWidth, &outHeight);

    return outWidth;
}

float Dialog::getTextHeight(const C2D_Text& text) {
    float outWidth;
    float outHeight;
    C2D_TextGetDimensions(&text, Dialog::fontSize, Dialog::fontSize, &outWidth, &outHeight);

    return outHeight;
}

float Dialog::getStringWidth(const string& string) {
    float outWidth;
    float outHeight;
    Dialog::getStringDimensions(string, outWidth, outHeight);

    return outWidth;
}

float Dialog::getStringHeight(const string& string) {
    float outWidth;
    float outHeight;
    Dialog::getStringDimensions(string, outWidth, outHeight);

    return outHeight;
}

void Dialog::split(const string& text, char delimiter, vector<string>& result) {
    string s = "";
    for (char x : text)
    {
        if (x == delimiter)
        {
            result.push_back(s);
            s = "";
        }
        else {
            s = s + x;
        }
    }
    result.push_back(s);
}