<?php
$device = $_GET['device'];
$action = $_GET['action'];

if ($device == 'role' || $device == 'neopixel') {
    $data = ['status' => false];

    if ($action == 'on') {
        $data['status'] = true;
    } elseif ($action == 'off') {
        $data['status'] = false;
    } elseif ($action == 'color' && $device == 'neopixel') {
        $data['status'] = true;
        $data['color'] = $_GET['color'];
    } elseif ($action == 'rainbow' && $device == 'neopixel') {
        $data['status'] = true;
        $data['mode'] = 'rainbow';
    }

    file_put_contents("{$device}_status.json", json_encode($data));
    echo json_encode(['success' => true]);
} else {
    echo json_encode(['success' => false]);
}
?>
