/*
ShakeSense
Team Members: Vibhav Gaka, Andrew Gerchak, Patrick Keenan, Advait Sepuri
Date Completed: 6/4/22
Firebase Functionality
*/

// Import the needed functions from the SDKs you need
import { initializeApp } from "https://www.gstatic.com/firebasejs/9.8.2/firebase-app.js";
import{getDatabase,ref,set,get,child,update,remove}
    from "https://www.gstatic.com/firebasejs/9.8.2/firebase-database.js";
 
//web app's Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyBHnBv1qAHbJ2onxq0IjQqk8VzmvjLA8qg",
    authDomain: "teststst-b9f37.firebaseapp.com",
    databaseURL: "https://teststst-b9f37-default-rtdb.firebaseio.com",
    projectId: "teststst-b9f37",
    storageBucket: "teststst-b9f37.appspot.com",
    messagingSenderId: "926685996764",
    appId: "1:926685996764:web:66cf3b899f3ef196def619"
  };
// Initialize Firebase
const app = initializeApp(firebaseConfig);
 
const db = getDatabase();
//set variables 
var memPhone, memPass, memPhone2, memPass2, x, y

// Sets variables to the values entered into the corresponding ids in html
function dataPrep(){
    memPhone = document.getElementById('number').value;
    memPass = document.getElementById('pass').value;
}

function dataPrep2(){
    memPhone2 = document.getElementById('number').value;
    memPass2 = document.getElementById('pass').value;
    x = document.getElementById("invisible");
    y = document.getElementById("invisible2");
}


//inserts data for a member into the database
function InsertData(){
    const dbref = ref(db)
    get(child(dbref,'member/'+ memPhone)).then((snapshot)=>{
        //if the path exists, then execute the code
        if(snapshot.exists()){
            alert('Phone number already signed up!');
        }
        else{
            //sets the reference paths in firebase to member/+memPhone
            set(ref(db,'member/'+ memPhone),{
                //set firebase data values to the variables from data prep
                memberPhone: memPhone,
                memberPassword: memPass
            })
            .then(()=>{
                alert('Signed Up!'); //alert successful order
            })
            .catch((error)=>{
                alert('unsuccessful, error'+error); //alert if an error occurred.
            });
        }
    })
    .catch((error)=>{
        alert('unsuccessful,error'+error);// alerts user if there is an error while executing
    })
}


function SelectData(){

    //take a snapshot of the firebase where the path, member /+memPhone, exists. 
    const dbref = ref(db);
    get(child(dbref,'member/'+ memPhone2)).then((snapshot)=>{
        //if the path exists, then execute the code
        if(snapshot.exists()){
            if(snapshot.val().memberPassword == memPass2){
                alert('Signed in!');    
                x.style.display = 'flex';
            }
            else{
                alert('No account found')
            }
        }
        else{
            alert('No account found') //alerts user if there is no such trip in database
        }
    })
    .catch((error)=>{
        alert('unsuccessful,error'+error);// alerts user if there is an error while executing
    })
}

//Start button for data collection
function Start(){
    const dbref = ref(db);
    get(child(dbref,'member/'+ memPhone2)).then((snapshot)=>{
        //if the path exists, then execute the code
        if(snapshot.exists()){
            if(snapshot.val().memberPassword == memPass2){
                alert('Data recording started!');
            }
        }
    })
    .catch((error)=>{
        alert('unsuccessful,error'+error);// alerts user if there is an error while executing
    })
}

//Stop button for data collection
function Stop(){
    const dbref = ref(db);
    get(child(dbref,'member/'+ memPhone2)).then((snapshot)=>{
        //if the path exists, then execute the code
        if(snapshot.exists()){
            if(snapshot.val().memberPassword == memPass2){
                alert('Data recording ended!');
                y.style.display = 'flex';

            }
        }    
    })
    .catch((error)=>{
        alert('unsuccessful,error'+error);// alerts user if there is an error while executing
    })
}

//Creates chart and inputs data
const labels = [];
for (let i = 0; i <= 64; i++) {
  labels[i] = i;
}

//Creates graph axes, labels, and parameters
new Chart("myChart", {
    type: 'line',
    data: {
        labels: labels,
        yAxisID: 'Severity',
        datasets: [{
            label: 'Tremor Severity',
            data: [0,0,1,1,1,3,2,2,2,2,2,2,3,3,5,5,5,4,4,4,5,5,5,6,5,5,3,3,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,1,1,3,4,5,5,5,5,4,4,3,3,2,2,2,1,0,2,2,1,1,1],
            fill: false,
            borderColor: 'rgb(75, 192, 192)',
            tension: 0.1
        }]
    },
    options: {}

});

//When the submit button is clicked, dataPrep() and InsertData() runs
document.getElementById('sign-up').onclick = function(){
    dataPrep();
    InsertData();
}

document.getElementById('log-in').onclick = function(){
    dataPrep2();
    SelectData();
}

document.getElementById('start').onclick = function(){
    dataPrep2();
    Start();
}

document.getElementById('stop').onclick = function(){
    dataPrep2();
    Stop();
}



