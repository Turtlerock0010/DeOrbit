if (get_isTagVisable()) {
  float* hubTagData = get_TagRow(0);
  float tagID = hubTagData[0];
  float centerx = hubTagData[1];
  float centery = hubTagData[2];
  float tagWidth = hubTagData[3];
  float tagHeight = hubTagData[4];
  float tagDistance = hubTagData[5];
  float tagAngle = hubTagData[6];

  PestoLink.printfTerminal("TAG ID: #%d\n| Distance: %.2fcm\n| Angle: %.2fDEG", tagID, tagDistance, tagAngle);
} else {
  PestoLink.printTerminal("No Tag Found.");
}
