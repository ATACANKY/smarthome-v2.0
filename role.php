<?php
$device = $_GET['device'];
$action = $_GET['action'];

if ($device == 'role') {
    $status = ($action == 'on') ? true : false;
    $data = ['status' => $status];
    file_put_contents('role_status.json', json_encode($data));
    echo json_encode(['success' => true]);
} else {
    echo json_encode(['success' => false]);
}
?>
