#include <string>

bool is_valid_alpha(char input) {
  if (input < 'a' || input > 'z')
    return false;
  else
    return true;
}

int return_digit(char input) {
  if (input < '1' || input > '9')
    return -1;
  else
    return (int) (input - 48);
}
