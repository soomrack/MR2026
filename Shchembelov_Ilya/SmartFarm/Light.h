class LightSensor{
  private:
  uint8_t _pin;
  uint8_t avarage_quantity = 250;
  uint16_t _raw_value;
  void _calc_lumen();
  public:
  uint16_t get_raw_value();
  uint16_t get_lumen_value();
  void set_avverage(uint8_t quantity);
}