<?php
$device = $_GET['device'];
$action = $_GET['action'];

if ($device == 'neopixel') {
    if ($action == 'color') {
        $color = $_GET['color'];
        $data = ['status' => true, 'color' => $color];
        file_put_contents('neopixel_status.json', json_encode($data));
        echo json_encode(['success' => true]);
    } elseif ($action == 'rainbow') {
        $data = ['status' => true, 'mode' => 'rainbow'];
        file_put_contents('neopixel_status.json', json_encode($data));
        echo json_encode(['success' => true]);
    } else {
        $status = ($action == 'on') ? true : false;
        $data = ['status' => $status];
        file_put_contents('neopixel_status.json', json_encode($data));
        echo json_encode(['success' => true]);
    }
} else {
    echo json_encode(['success' => false]);
}
?>
