<?php

$json_raw = file_get_contents('php://input');

$payload = json_decode($json_raw,true);

$device_id = $payload['dev_id'];

//https://integrations.thethingsnetwork.org/ttn-eu/api/v2/down/dht11_ttgo/ttgo?key=ttn-account-v2.RTS-lfZkaNtND4GGvUU5v-1AorPqfL_f6-QjPX9XU9M
//API Url
$downlinkurl = 'https://integrations.thethingsnetwork.org/ttn-eu/api/v2/down/dht11_ttgo/ttgo?key=ttn-account-v2.RTS-lfZkaNtND4GGvUU5v-1AorPqfL_f6-QjPX9XU9M';
 
//Execute the request
if( $device_id == 'ttgo_node') {
    $payload_fields = $payload['payload_fields'];
    $temperature = $payload_fields['temperature'];
    $humidity = $payload_fields['humidity'];    
        //Initiate cURL.
    $ch = curl_init($downlinkurl);
    
    //The JSON data.
    $jsonData = array(
        'dev_id' => 'the_things_uno',
        'port' => 1,
        'confirmed' => false,
        'payload_fields' => array(
            'temperature' => $temperature,
            'humidity' => $humidity,
        ),
        'schedule' => 'replace'
    );
    
    //Encode the array into JSON.
    $jsonDataEncoded = json_encode($jsonData);
    
    //Tell cURL that we want to send a POST request.
    curl_setopt($ch, CURLOPT_POST, 1);
    
    //Attach our encoded JSON string to the POST fields.
    curl_setopt($ch, CURLOPT_POSTFIELDS, $jsonDataEncoded);
    
    //Set the content type to application/json
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: application/json')); 

    file_get_contents("https://api.thingspeak.com/update?api_key=IW29EZMGRFMFCFWP&field1=" . $temperature . "&field2=" . $humidity);
    $result = curl_exec($ch);
}


?>