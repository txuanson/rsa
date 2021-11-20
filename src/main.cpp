#include "BigInt.h"

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string hex = "b";
    string hex1 = "8";
    BigInt n, m, ans;
    n.fromHex(hex);
    m.fromHex(hex1);
    ans = n * m;
    cout << "ans: " << ans.toHex();
}