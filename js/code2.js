/*
ShakeSense
Team Members: Vibhav Gaka, Andrew Gerchak, Patrick Keenan, Advait Sepuri
Date Completed: 5/29/22
Website functions (Scrolling Nav bar, modals, buttons)
*/

// code for showing white nav bar as you scroll
window.addEventListener('scroll', function () {
    let nav = document.querySelector('nav');

    let arrow = document.querySelector('.arrow');

    arrow.classList.toggle('scrolling-active2', window.scrollY > 250);

    nav.classList.toggle('scrolling-active', window.scrollY > 50); //adds the class scrolling active to nav when scrollY>50
})

render();
// code for modals
var modalBtns = document.querySelectorAll('.modal-open'); //returns all elements that math the selector '.modal-open'

var modalClose = document.querySelectorAll('.modal-close'); //returns all elements that math the selector '.modal-close'

//each time an image with the class modal open is pressed, the function runs
modalBtns.forEach(function(btn){
    btn.onclick=function(){
        var modal = btn.getAttribute('data-modal'); //var modal equals the attribute that data-modal is set to

        document.getElementById(modal).style.display = "block"; //displays the modal as a block which makes it appear

    };
}); 
//each time the close button is pressed, the function runs
modalClose.forEach(function(btn){
    btn.onclick=function(){
        btn.closest(".modal").style.display = "none"; //sets the modal's display to none which makes it disappear
    };
});

