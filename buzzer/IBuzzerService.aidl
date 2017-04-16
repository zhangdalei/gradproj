package android.os;

/** {@hide} */
interface IBuzzerService
{
    int buzzerCtrl(int cmd,long frec);
}