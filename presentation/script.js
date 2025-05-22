document.addEventListener('DOMContentLoaded', () => {
  const slides = document.querySelectorAll('.slide');
  const prevBtn = document.getElementById('prevBtn');
  const nextBtn = document.getElementById('nextBtn');
  const prevBtn2 = document.getElementById('prevBtn2');
  const nextBtn2 = document.getElementById('nextBtn2');
  const prevBtn4 = document.getElementById('prevBtn4');
  const nextBtn4 = document.getElementById('nextBtn4');
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

  // Keyboard navigation
  document.addEventListener('keydown', (e) => {
    if (e.key === 'ArrowRight') nextSlide();
    if (e.key === 'ArrowLeft') prevSlide();
  });

  // Initialize - show first slide
  showSlide(0);
}); 