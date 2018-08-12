/**************************************************************************//**
* @file BLDC_Main.h
* @brief Rangefinder main module
* @author Ian Glass
* @version 3.20.2
* @date    28-July-2018
* @brief   Main module for the Rangefinder
*******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************/

/* Defines -------------------------------------------------------------------*/
#ifndef PWM_PORT
#define PWM_PORT	gpioPortD
#endif
#ifndef PWM_PIN
#define PWM_PIN		6
#endif

/* defines number of startup pulses, drive for 2 secs */
#define StartupStop	200
/* Systick all frequency */
#define SYS_FREQ 1000
/* PWM ISR call frequency. Need 0.1MHz to achieve 1% resolution in PWM output for Hyperion BLDC */
#define PWM_FREQ 1600000
/* Time offset to start motor spinning, in 10us increments */
#define Offset 100

