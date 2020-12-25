import Vue from 'vue'
import App from './App.vue'
import VueJsModal from 'vue-js-modal'
import 'vue-js-modal/dist/styles.css'

Vue.config.productionTip = false

Vue.use(VueJsModal)

new Vue({
  render: h => h(App)
}).$mount('#app')
