document.addEventListener('DOMContentLoaded', () => {
  const slides = document.querySelectorAll('.slide');
  const prevBtn = document.getElementById('prevBtn');
  const nextBtn = document.getElementById('nextBtn');
  const prevBtn2 = document.getElementById('prevBtn2');
  const nextBtn2 = document.getElementById('nextBtn2');
  const prevBtn4 = document.getElementById('prevBtn4');
  const nextBtn4 = document.getElementById('nextBtn4');
  const prevBtn6 = document.getElementById('prevBtn6');
  const nextBtn6 = document.getElementById('nextBtn6');
  const prevBtn7 = document.getElementById('prevBtn7');
  const nextBtn7 = document.getElementById('nextBtn7');
  const prevBtn8 = document.getElementById('prevBtn8');
  const nextBtn8 = document.getElementById('nextBtn8');
  let current = 0;

  function showSlide(idx) {
    slides.forEach((slide, i) => {
      slide.classList.toggle('active', i === idx);
    });
    current = idx;
  }

  function nextSlide() {
    current = (current + 1) % slides.length;
    showSlide(current);
  }

  function prevSlide() {
    current = (current - 1 + slides.length) % slides.length;
    showSlide(current);
  }

  // Navigation buttons
  prevBtn.addEventListener('click', prevSlide);
  nextBtn.addEventListener('click', nextSlide);

  // Slide 2 navigation buttons
  if (prevBtn2 && nextBtn2) {
    prevBtn2.addEventListener('click', prevSlide);
    nextBtn2.addEventListener('click', nextSlide);
  }

  // Slide 4 navigation buttons
  if (prevBtn4 && nextBtn4) {
    prevBtn4.addEventListener('click', prevSlide);
    nextBtn4.addEventListener('click', nextSlide);
  }

  // Slide 6 navigation buttons
  if (prevBtn6 && nextBtn6) {
    prevBtn6.addEventListener('click', prevSlide);
    nextBtn6.addEventListener('click', nextSlide);
  }

  // Add new handlers for slide 7 if we add navigation buttons later
  if (prevBtn7 && nextBtn7) {
    prevBtn7.addEventListener('click', prevSlide);
    nextBtn7.addEventListener('click', nextSlide);
  }

  // Add new handlers for slide 8
  if (prevBtn8 && nextBtn8) {
    prevBtn8.addEventListener('click', prevSlide);
    nextBtn8.addEventListener('click', nextSlide);
  }

  // Keyboard navigation
  document.addEventListener('keydown', (e) => {
    if (e.key === 'ArrowRight') nextSlide();
    if (e.key === 'ArrowLeft') prevSlide();
  });

  // Initialize - show first slide
  showSlide(0);
}); 