void chirp(int ms) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(ms);
  digitalWrite(BUZZER_PIN, LOW);
}

void chirp_loop(int on_ms, int off_ms, int count) {
        for (int i = 0; i < count; i++)
      {
        chirp(on_ms);
        delay(off_ms);
      }
}